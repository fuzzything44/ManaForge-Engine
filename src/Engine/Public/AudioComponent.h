#pragma once

#include "Engine.h"
#include "SceneComponent.h"

class SoundCue;
class SoundSource;

class AudioComponent : public SceneComponent
{
public:
	ENGINE_API explicit AudioComponent(Actor* owner = nullptr, Transform trans = Transform{}, SoundCue* cue = nullptr);
private:
	SoundSource* source;
};