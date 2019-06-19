#ifndef __FONCTIONS_SON

#define __FONCTIONS_SON

#include <SDL/SDL_mixer.h>

#define NB_MUSIQUES 4
#define NB_EFFETS 3

#define MIX_FORMAT_DRAPEAUX MIX_INIT_MOD | MIX_INIT_MP3

#define MIX_TAMPON_SON 1024 /* en octets */

typedef struct {
	Mix_Music *musiques[NB_MUSIQUES];
	Mix_Music *effets[NB_EFFETS];
} BanqueSonore;


#endif