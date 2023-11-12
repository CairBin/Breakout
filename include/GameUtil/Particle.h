#ifndef _PARTICLE_H_
#define _PARTOCLE_H_

#include <vector>
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "GameUtil/Texture.h"
#include "GameUtil/GameObject.h"

namespace GameUtil{
    struct Particle{
        glm::vec2 Position;
        glm::vec2 Velocity;
        glm::vec4 Color;
        GLfloat Life;

        Particle():Position(0.0f),Velocity(0.0f),Color(1.0f),Life(0.0f){}
    };

    class ParticleGenerator{
    public:
        ParticleGenerator(Shader shader,Texture2d texture,GLuint amount);
        void Update(GLfloat dt,GameObject& object,GLuint newParticles,glm::vec2 offset=glm::vec2(0.0f,0.0f));
        void Draw();
    private:
        std::vector<Particle> particles;
        GLuint amount;
        Shader shader;
        Texture2d texture;
        GLuint VAO;
        void init();
        GLuint firstUnusedParticle();
        void respawnParticle(Particle &particle, GameObject &object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
    };
}


#endif