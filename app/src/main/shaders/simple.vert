/*
 * Copyright 2020 Konstantinos Tzevanidis
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 i_position;
layout(location = 1) in vec2 i_tex_coords;

layout(location = 0) out vec2 o_tex_coords;

void main(){
    gl_Position = i_position;
    o_tex_coords = i_tex_coords;
}