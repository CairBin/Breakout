#ifndef _AUDIO_H_
#define _AUDIO_H_

#include <cstdlib>

#include "yse/yse.hpp"

#if defined YSE_WINDOWS
#include <conio.h>
#else
#include "wincompat.h"
#endif

#include "yse/system.hpp"
#include "yse/io.hpp"
#include "yse/listener.hpp"
#include "yse/classes.hpp"
#include "yse/log.hpp"

#include "glad/glad.h"
#include "glm/glm.hpp"

namespace GameUtil{
    class Audio{
        public:
            static void Init();
            static void Update();
            static void Close();
            static void SetMaxSound(GLuint val);

        private:
            YSE::sound* sound;

        public:
            Audio(const char *filename);
            Audio(const char *filename, GLboolean isLoop);
            ~Audio();
            Audio& Play();
            Audio &SetPosition(glm::vec3 pos);
            Audio &SetRelative(GLboolean val);
            Audio &SetDoppler(GLboolean val);
            Audio &SetLoop(GLboolean val);
            Audio &Pause();
            Audio &Stop();
    };
}

#endif