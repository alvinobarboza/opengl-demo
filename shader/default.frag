#version 330 core

out vec4 FragColor;

in vec3 color;
in vec2 texCoord;
in vec3 normal;
in vec3 current_pos;

uniform sampler2D tex0;
uniform sampler2D tex1;

uniform vec4 light_color;
uniform vec3 light_pos;
uniform vec3 cam_pos;

void main()
{
    float ambient = 0.20f;

    vec3 normal_n = normalize(normal);
    vec3 light_direction = normalize(light_pos - current_pos);

    float diffuse = max(dot(normal_n, light_direction), 0.0f);

    float specular_light = 0.5f;
    vec3 view_dir = normalize(cam_pos - current_pos);
    vec3 reflection_dir = reflect(-light_direction, normal_n);
    float spec_amount = pow(max(dot(view_dir, reflection_dir), 0.0f), 16);
    float specular = spec_amount * specular_light;

    FragColor = texture(tex0, texCoord) * light_color * (diffuse + ambient) + texture(tex1, texCoord).r * specular;
}