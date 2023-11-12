#include "GameUtil/Audio.h"

namespace GameUtil{
    void Audio::Init(){
        YSE::System().init();
    }
    void Audio::Update(){
        YSE::System().update();
    }

    void Audio::Close(){
        YSE::System().close();
    }

    void Audio::SetMaxSound(GLuint val){
        YSE::System().maxSounds(val);
    }

    Audio::Audio(const char* filename)
        :sound(nullptr){
        this->sound = new YSE::sound;
        sound->create(filename);
    }

    Audio::Audio(const char* filename,GLboolean isLoop)
        :sound(nullptr){
        this->sound = new YSE::sound;
        sound->create(filename, nullptr, isLoop);
    }

    Audio& Audio::Play(){
        this->sound->play();
        return *this;
    }

    Audio& Audio::SetDoppler(GLboolean val){
        this->sound->setDoppler(val);
        return *this;
    }

    Audio& Audio::SetRelative(GLboolean val){
        this->sound->setRelative(val);
        return *this;
    }

    Audio& Audio::SetLoop(GLboolean val){
        this->sound->setLooping(val);
        return *this;
    }

    Audio& Audio::Stop(){
        this->sound->stop();
        return *this;
    }

    Audio& Audio::Pause(){
        this->sound->pause();
        return *this;
    }

    Audio& Audio::SetPosition(glm::vec3 pos){
        this->sound->setPosition(YSE::Vec(pos.x, pos.y, pos.z));
        return *this;
    }

    Audio::~Audio(){
        delete sound;
    }
}