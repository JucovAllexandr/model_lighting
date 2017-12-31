#version 330
in vec3 objectColor;
in vec3 FragPos;
in vec3 Normal;
in vec2 tex_coords;
uniform vec3 lPos;
uniform vec3 camPos;
uniform int lightning;
uniform sampler2D texture;
uniform bool hasTexture;
void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lPos - FragPos);
    vec3 lightColor = vec3(1,1,1);
    float ambientStrength = 0.2;
    //vec3 ambient = ambientStrength * lightColor * vec3(1,0,0) ;
    vec3 ambient;
    if(hasTexture)
        ambient = ambientStrength * lightColor * texture2D(texture,tex_coords).rgb ;
    else
        ambient = ambientStrength * lightColor * vec3(1,0,0);
    vec3 diffuse;
    float diff = max(dot(norm, lightDir), 0.0);

    if(lightning == 1 ||lightning == 2)
        diffuse = diff * lightColor;

    if(lightning == 3)
        if (diff > 0.95) {
            diffuse = lightColor;
        } else if (diff > 0.5) {
            diffuse = lightColor * 0.7;
        } else if (diff > 0.2) {
            diffuse = lightColor * 0.2;
        } else {
            diffuse = lightColor * 0.05;
        }



    float specularStrength = 0.5;
    vec3 camDir = normalize(camPos-FragPos);
    vec3 reflectDir = reflect(-lightDir,norm);

    float spec = pow(max(dot(camDir, reflectDir), 0.0), 64);
    vec3 specular = specularStrength * spec * lightColor;
    //vec3 result = (ambient+diffuse+specular) * vec3(1,0,0);
    vec3 result;
    if(lightning == 1){
        if(hasTexture)
            result = (ambient+diffuse+specular) * texture2D(texture,tex_coords).rgb;
        else
            result = (ambient+diffuse+specular) * vec3(1,0,0);
        gl_FragColor =  vec4(result, 1.0);
    }
    if(lightning == 3 || lightning == 2)
        if(hasTexture)
            gl_FragColor =  vec4((diffuse+ambient)*texture2D(texture,tex_coords).rgb, 1.0);
        else
            result = (ambient+diffuse) * vec3(1,0,0);

    //gl_FragColor = texture2D(texture,tex_coords);
}
