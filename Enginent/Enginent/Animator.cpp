//#include "Animator.h"
//
//void Animator::AddAnimation(Animation* animation) {
//	animationList.insert(std::pair<std::string, Animation*>(animation->animationName, animation));
//}
//
//
//void Animator::PlayAnimation(std::string name) {
//	// reset current animation frame to zero
//	currentAnim->ResetAnimation();
//	currentPlayingName = name;
//	currentAnim = animationList[name];
//}
//
//
//std::string Animator::GetCurrentPlayingName() {
//	return currentPlayingName;
//}