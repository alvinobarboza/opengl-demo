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

// won't do directional....

vec4 point_light()
{
    vec3 light_vec = light_pos - current_pos;
    float dist = length(light_vec);
    float a = 3.0;
    float b = 0.7;
    float intern = 1.0f / (a * dist * dist + b * dist + 1.0f);

    float ambient = 0.20f;

    vec3 normal_n = normalize(normal);
    vec3 light_direction = normalize(light_vec);

    float diffuse = max(dot(normal_n, light_direction), 0.0f);

    float specular_light = 0.5f;
    vec3 view_dir = normalize(cam_pos - current_pos);
    vec3 reflection_dir = reflect(-light_direction, normal_n);
    float spec_amount = pow(max(dot(view_dir, reflection_dir), 0.0f), 16);
    float specular = spec_amount * specular_light;

    return texture(tex0, texCoord) * light_color * (diffuse * intern + ambient) + texture(tex1, texCoord).r * specular * intern;
}

vec4 spot_light()
{
    float outer_cone = 0.92f;
    float inner_cone = 0.95f;

    float ambient = 0.20f;

    vec3 light_vec = light_pos - current_pos;
    vec3 normal_n = normalize(normal);
    vec3 light_direction = normalize(light_vec);

    float diffuse = max(dot(normal_n, light_direction), 0.0f);

    float specular_light = 0.5f;
    vec3 view_dir = normalize(cam_pos - current_pos);
    vec3 reflection_dir = reflect(-light_direction, normal_n);
    float spec_amount = pow(max(dot(view_dir, reflection_dir), 0.0f), 16);
    float specular = spec_amount * specular_light;

    float angle = dot(vec3(0.0f, -1.0f, 0.0f), -light_direction);
    float intensity = clamp((angle - outer_cone) / (inner_cone - outer_cone), 0.0f, 1.0f);

    return (texture(tex0, texCoord) * (diffuse * intensity + ambient) + texture(tex1, texCoord).r * specular * intensity) * light_color;
}

void main()
{
    FragColor = spot_light();
}