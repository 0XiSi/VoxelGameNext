#include "Shader.hpp"



Shader::Shader() {
    programID = 0;
    uniformModel = 0;
    uniformProjection = 0;
    uniformView = 0;

    pointLightCount = 0;
    spotLightCount = 0;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode) {
    CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation) {
    std::string vertexString = ReadFile(vertexLocation);
    std::string fragmentString = ReadFile(fragmentLocation);
    const char* vertexCode = vertexString.c_str();
    const char* fragmentCode = fragmentString.c_str();

    CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation) {
    std::string vertexString = ReadFile(vertexLocation);
    std::string geometryString = ReadFile(geometryLocation);
    std::string fragmentString = ReadFile(fragmentLocation);
    const char* vertexCode = vertexString.c_str();
    const char* geometryCode = geometryString.c_str();
    const char* fragmentCode = fragmentString.c_str();

    CompileShader(vertexCode, geometryCode, fragmentCode);

}

std::string Shader::ReadFile(const char* fileLocation) {
    std::string content;
    std::ifstream fileStream(fileLocation, std::ios::in);

    if (!fileStream.is_open()) {
        printf("Failed to read %s! File doesn't exist.", fileLocation);
        return "";
    }

    std::string line = "";
    while (!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}


void Shader::CompileShader(const char* vertexCode, const char* fragmentCode) {
    programID = glCreateProgram();

    if (!programID) {
        printf("Error creating shader program!\n");
        return;
    }

    AddShader(programID, vertexCode, GL_VERTEX_SHADER);
    AddShader(programID, fragmentCode, GL_FRAGMENT_SHADER);

    CompileProgram();
}

void Shader::CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode) {
    programID = glCreateProgram();

    if (!programID) {
        printf("Error creating shader program!\n");
        return;
    }

    AddShader(programID, vertexCode, GL_VERTEX_SHADER);
    AddShader(programID, geometryCode, GL_GEOMETRY_SHADER);
    AddShader(programID, fragmentCode, GL_FRAGMENT_SHADER);

    CompileProgram();
}

GLuint Shader::GetProjectionLocation() {
    return uniformProjection;
}
GLuint Shader::GetModelLocation() {
    return uniformModel;
}
GLuint Shader::GetViewLocation() {
    return uniformView;
}
GLuint Shader::GetAmbientColorLocation() {
    return uniformDirectionalLight.uniformColor;
}
GLuint Shader::GetDiffuseIntensityLocation() {
    return uniformDirectionalLight.uniformDiffuseIntensity;
}
GLuint Shader::GetDirectionLocation() {
    return uniformDirectionalLight.uniformDirection;
}
GLuint Shader::GetSpecularIntensityLocation() {
    return uniformSpecularIntensity;
}
GLuint Shader::GetShininessLocation() {
    return uniformShininess;
}
GLuint Shader::GetEyePositionLocation() {
    return uniformEyePosition;
}
GLuint Shader::GetOmniLightPosLocation() {
    return uniformOmniLightPos;
}
GLuint Shader::GetFarPlaneLocation() {
    return uniformFarPlane;
}
GLuint Shader::GetAmbientIntensityLocation() {
    return uniformDirectionalLight.uniformAmbientIntensity;
}

void Shader::SetDirectionalLight(DirectionalLight* dLight) {
    dLight->UseLight(
        uniformDirectionalLight.uniformAmbientIntensity,
        uniformDirectionalLight.uniformColor,
        uniformDirectionalLight.uniformDiffuseIntensity,
        uniformDirectionalLight.uniformDirection
    );
}

void Shader::SetPointLights(PointLight* pLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset) {
    if (lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;

    glUniform1i(uniformPointLightCount, lightCount);

    for (size_t i = 0; i < lightCount; i++) {
        pLight[i].UseLight(
            uniformPointLight[i].uniformAmbientIntensity,
            uniformPointLight[i].uniformColor,
            uniformPointLight[i].uniformDiffuseIntensity,
            uniformPointLight[i].uniformPosition,
            uniformPointLight[i].uniformConstant,
            uniformPointLight[i].uniformLinear,
            uniformPointLight[i].uniformExponent
        );
        pLight[i].GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
        glUniform1i(uniformOmniShadowMaps[i + offset].shadowMap, textureUnit + i);
        glUniform1f(uniformOmniShadowMaps[i + offset].farPlane, pLight[i].GetFarPlane());
    }

}

void Shader::SetSpotLights(SpotLight* sLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset) {
    if (lightCount > MAX_SPOT_LIGHTS) lightCount = MAX_SPOT_LIGHTS;

    glUniform1i(uniformSpotLightCount, lightCount);

    for (size_t i = 0; i < lightCount; i++) {
        sLight[i].UseLight(
            uniformSpotLight[i].uniformAmbientIntensity,
            uniformSpotLight[i].uniformColor,
            uniformSpotLight[i].uniformDiffuseIntensity,
            uniformSpotLight[i].uniformPosition,
            uniformSpotLight[i].uniformDirection,
            uniformSpotLight[i].uniformConstant,
            uniformSpotLight[i].uniformLinear,
            uniformSpotLight[i].uniformExponent,
            uniformSpotLight[i].uniformEdge
        );
        sLight[i].GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
        glUniform1i(uniformOmniShadowMaps[i + offset].shadowMap, textureUnit + i);
        glUniform1f(uniformOmniShadowMaps[i + offset].farPlane, sLight[i].GetFarPlane());
    }

}

void Shader::SetTexture(GLuint textureUnit) {
    glUniform1i(uniformTexture, textureUnit);

}

void Shader::SetDirectionalShadowMap(GLuint textureUnit) {
    glUniform1i(uniformDirectionalShadowMap, textureUnit);

}

void Shader::SetDirectionalLightTransform(glm::mat4* lTransform) {
    glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*lTransform));
}

void Shader::SetLightMatrices(std::vector<glm::mat4> lightMatrices) {
    for (size_t i = 0; i < 6; i++) {
        glUniformMatrix4fv(uniformLightMatrices[i], 1, GL_FALSE, glm::value_ptr(lightMatrices[i]));
    }
}


void Shader::UseShader() {
    glUseProgram(programID);
}

void Shader::ClearShader() {
    if (programID != 0) {
        glDeleteProgram(programID);
        programID = 0;
    }

    uniformModel = 0;
    uniformProjection = 0;
}

void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {
    GLuint theShader = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = static_cast<GLint>(strlen(shaderCode));

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
        return;
    }

    glAttachShader(theProgram, theShader);
}

void Shader::CompileProgram() {
    glLinkProgram(programID);

    glUseProgram(programID);
    uniformModel = glGetUniformLocation(programID, "model");
    uniformProjection = glGetUniformLocation(programID, "projection");
    uniformView = glGetUniformLocation(programID, "view");
    uniformDirectionalLight.uniformColor = glGetUniformLocation(programID, "directionalLight.base.color");
    uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(programID, "directionalLight.base.ambientIntensity");
    uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(programID, "directionalLight.base.diffuseIntensity");
    uniformDirectionalLight.uniformDirection = glGetUniformLocation(programID, "directionalLight.direction");
    uniformSpecularIntensity = glGetUniformLocation(programID, "material.specularIntensity");
    uniformShininess = glGetUniformLocation(programID, "material.shininess");
    uniformEyePosition = glGetUniformLocation(programID, "eyePosition");

    uniformPointLightCount = glGetUniformLocation(programID, "pointLightCount");

    for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
        char locBuff[100] = { '\0' };

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.color", i);
        uniformPointLight[i].uniformColor = glGetUniformLocation(programID, locBuff);
        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
        uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(programID, locBuff);
        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
        uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(programID, locBuff);
        /*-----------------------------------------------------------------------------------
        -----------------------------------------------------------------------------------*/
        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
        uniformPointLight[i].uniformPosition = glGetUniformLocation(programID, locBuff);
        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
        uniformPointLight[i].uniformConstant = glGetUniformLocation(programID, locBuff);
        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
        uniformPointLight[i].uniformLinear = glGetUniformLocation(programID, locBuff);
        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i);
        uniformPointLight[i].uniformExponent = glGetUniformLocation(programID, locBuff);
    }

    uniformSpotLightCount = glGetUniformLocation(programID, "spotLightCount");

    for (int i = 0; i < MAX_SPOT_LIGHTS; i++) {
        char locBuff[100] = { '\0' };

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.color", i);
        uniformSpotLight[i].uniformColor = glGetUniformLocation(programID, locBuff);
        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambientIntensity", i);
        uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(programID, locBuff);
        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", i);
        uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(programID, locBuff);
        /*-----------------------------------------------------------------------------------
        -----------------------------------------------------------------------------------*/
        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", i);
        uniformSpotLight[i].uniformPosition = glGetUniformLocation(programID, locBuff);
        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", i);
        uniformSpotLight[i].uniformConstant = glGetUniformLocation(programID, locBuff);
        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", i);
        uniformSpotLight[i].uniformLinear = glGetUniformLocation(programID, locBuff);
        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.exponent", i);
        uniformSpotLight[i].uniformExponent = glGetUniformLocation(programID, locBuff);
        /*-----------------------------------------------------------------------------------
        -----------------------------------------------------------------------------------*/
        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", i);
        uniformSpotLight[i].uniformDirection = glGetUniformLocation(programID, locBuff);
        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", i);
        uniformSpotLight[i].uniformEdge = glGetUniformLocation(programID, locBuff);
    }

    uniformTexture = glGetUniformLocation(programID, "theTexture");
    uniformDirectionalLightTransform = glGetUniformLocation(programID, "directionalLightTransform");
    uniformDirectionalShadowMap = glGetUniformLocation(programID, "directionalShadowMap");


    uniformOmniLightPos = glGetUniformLocation(programID, "lightPos");
    uniformFarPlane = glGetUniformLocation(programID, "farPlane");

    for (size_t i = 0; i < 6; i++) {
        char locBuff[100] = { '\0' };

        snprintf(locBuff, sizeof(locBuff), "lightMatrices[%d]", i);
        uniformLightMatrices[i] = glGetUniformLocation(programID, locBuff);
    }

    for (size_t i = 0; i < MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS; i++) {
        char locBuff[100] = { '\0' };

        snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d]", i);
        uniformOmniShadowMaps[i].shadowMap = glGetUniformLocation(programID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "omniShadowFarPlanes[%d]", i);
        uniformOmniShadowMaps[i].farPlane = glGetUniformLocation(programID, locBuff);
    }
}

Shader::~Shader() {
    ClearShader();
}

bool Shader::logStatus(
    GLuint objectID,
    PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
    PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
    GLenum statusType
) {
    GLint status;
    objectPropertyGetterFunc(objectID, statusType, &status);
    if (status != GL_TRUE) {
        GLint logLength;
        objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &logLength);
        GLchar* log = new GLchar[logLength];
        getInfoLogFunc(objectID, logLength, NULL, log);
        std::cout << "Shader linking failed: " << log << std::endl;
        delete[] log;
        return false;
    }
    return true;
}
bool Shader::logShaderError(GLuint programID) {
    return logStatus(programID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}
bool Shader::logProgramError(GLuint programID) {
    return logStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}
bool Shader::Validate() {
    return logProgramError(programID);
}
