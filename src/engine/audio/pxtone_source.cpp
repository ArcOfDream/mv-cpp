#include "mv/audio/pxtone_source.h"
#include "soloud.h"
#include "soloud_audiosource.h"
#include "soloud_error.h"
#include "pxtnError.h"
#include "pxtnService.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <vector>

namespace SoLoud {

const float float_divisor = 1.0 / INT16_MAX;

bool px_read(void *user, void *p_dst, int32_t size, int32_t num) {
    int32_t i = fread(p_dst, size, num, (FILE *)user);
    if (i < num) {
        return false;
    }
    return true;
}

bool px_write(void *user, const void *p_dst, int32_t size, int32_t num) {
    int32_t i = fwrite(p_dst, size, num, (FILE *)user);
    if (i < num) {
        return false;
    }
    return true;
}

bool px_seek(void *user, int32_t mode, int32_t size) {
    if (fseek((FILE *)user, size, mode) < 0) {
        return false;
    }
    return true;
}

bool px_pos(void *user, int32_t *p_pos) {
    int32_t i = ftell((FILE *)user);
    if (i < 0) {
        return false;
    }
    *p_pos = i;
    return true;
}

PXTone::PXTone() {
	mChannels = 2;
	mBaseSamplerate = 44100.0f;
	
}

AudioSourceInstance* PXTone::createInstance() {
    if (!stream) return 0;

	return new PXToneInstance{this, stream};
}

AudioSourceInstance* PXTone::create_instance() {
	return createInstance();
}

result PXTone::load(const char* a_filename) {
	stream = fopen(a_filename, "rb");
	if(!stream) return FILE_LOAD_FAILED;

	return SO_NO_ERROR;
}

PXToneInstance::PXToneInstance(PXTone *parent, FILE *px_stream) {
	s_parent = parent;
    stream = px_stream;
    mChannels = parent->mChannels;
    mBaseSamplerate = parent->mBaseSamplerate;
	mSamplerate = mBaseSamplerate * mSetRelativePlaySpeed * mChannels / 2;

    // setting up the pxtone service
    service = new pxtnService(px_read, px_write, px_seek, px_pos);
    err = service->init();
    if (err != pxtnOK ) {
        fprintf(stderr, "ERROR: Failed to initialize the library: %s\n", pxtnError_get_string(err));
        // fclose(stream);
        delete service;
        done = true;
        fflush(stderr);
        return;
    }

    // set destination quality for the service
    if (!service->set_destination_quality(mChannels, mSamplerate)) {
        fprintf(stderr, "ERROR: Failed to set the quality!");
        // delete service;
        fclose(stream);
        done = true;
        fflush(stderr);
        return;
    }

    // try to read the file
    err = service->read(stream);
    if (err != pxtnOK ) {
        fprintf(stderr, "ERROR: Failed to set the source stream: %s\n", pxtnError_get_string(err));
        // delete service;
        fclose(stream);
        done = true;
        fflush(stderr);
        return;
    }

    err = service->tones_ready();
    if (err != pxtnOK) {
        fprintf(stderr, "ERROR: Failed to initialize tones: %s\n", pxtnError_get_string(err));
        // delete service;
        fclose(stream);
        done = true;
        fflush(stderr);
        return;
    }

    service->moo_get_total_sample();

    memset(&prep, 0, sizeof(pxtnVOMITPREPARATION));
    prep.flags |= pxtnVOMITPREPFLAG_loop | pxtnVOMITPREPFLAG_unit_mute;
    prep.fadein_sec = 5;
    prep.start_pos_float = 0;
    prep.master_volume = 1.0f;

    if (!service->moo_preparation(&prep)) {
        service->evels->Release();
        fclose(stream);
        fprintf(stderr, "ERROR: Failed to initialize the moo\n");
        // delete service;
        done = true;
        fflush(stderr);
        return;
    }
}

PXToneInstance::~PXToneInstance() {
    done = true;
    // for(int i = 0; i < FILTERS_PER_STREAM; i++) {
    //     delete mFilter[i];
    // }
    if(service) delete service;
    // fclose(stream);
}

unsigned int PXToneInstance::getAudio(float *a_buffer, unsigned int a_samples, unsigned int) {
    if(!service) return 0;

	int written_smp = 0;
	int16_t size = a_samples*mChannels  ;
	std::vector<int16_t> intermediate_buffer = std::vector<int16_t>(size);

    if (!service->Moo(intermediate_buffer.data(), size*sizeof(int16_t), &written_smp)) done = true;

	for(unsigned int i=0; i < a_samples; i++) {

		a_buffer[i] =           (float)intermediate_buffer[i*mChannels] * float_divisor * mOverallVolume;

        if (mChannels >= 2)
        a_buffer[a_samples+i] = (float)intermediate_buffer[i*mChannels+1] * float_divisor * mOverallVolume;
	}

	return written_smp;
 }

bool PXToneInstance::hasEnded() { return done; }

unsigned int PXToneInstance::get_audio(float *a_buffer, unsigned int a_samples, unsigned int sample_size) {
	return getAudio(a_buffer, a_samples, sample_size);
}

bool PXToneInstance::has_ended() { return hasEnded(); }

result PXToneInstance::seek(time , float *, unsigned int ) {
	return NOT_IMPLEMENTED;
}

result PXToneInstance::rewind() {
	return NOT_IMPLEMENTED;
}

} // namespace SoLoud
