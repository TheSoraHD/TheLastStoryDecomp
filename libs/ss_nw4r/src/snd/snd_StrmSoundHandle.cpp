#include "nw4r/snd/StrmSoundHandle.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_StrmSoundHandle.cpp
 */

/*******************************************************************************
 * headers
 */

#include "common.h" // nullptr

#include "nw4r/snd/SoundHandle.h"
#include "nw4r/snd/StrmSound.h"
#include "nw4r/ut/RuntimeTypeInfo.h"

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd {

StrmSoundHandle::StrmSoundHandle(SoundHandle* handle)
	: mSound(nullptr)
{
	if (!handle)
		return;

	detail::BasicSound *basicSound = handle->detail_GetAttachedSound();
	if (!basicSound)
		return;

	if (detail::StrmSound *sound =
	        ut::DynamicCast<detail::StrmSound *>(basicSound))
	{

		mSound = sound;

		if (mSound->IsAttachedTempSpecialHandle())
			mSound->DetachTempSpecialHandle();

		mSound->mTempSpecialHandle = this;
	}
}

void StrmSoundHandle::DetachSound()
{
	if (IsAttachedSound())
	{
		if (mSound->mTempSpecialHandle == this)
			mSound->mTempSpecialHandle = nullptr;
	}

	if (mSound)
		mSound = nullptr;
}

}} // namespace nw4r::snd
