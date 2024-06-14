#include "pxtnService.h"
#include "soloud.h"
#include "soloud_file.h"
#include <stdio.h>

#pragma once

// Going to make aliases for derived functions to make up for
// the differences in coding style.

namespace SoLoud {

bool px_read(void *user, void *p_dst, long size, long num);
bool px_write(void *user, const void *p_dst, long size, long num);
bool px_seek(void *user, long mode, long size);
bool px_pos(void *user, long p_pos);

class PXTone : public AudioSource {
    FILE* stream;
  public:
    PXTone();
    virtual AudioSourceInstance *createInstance();
    AudioSourceInstance *create_instance();

    result load(const char* a_filename);
    // result load_file(File *a_file);
};

class PXToneInstance : public AudioSourceInstance {
    PXTone *s_parent;
    pxtnService *service;
    pxtnVOMITPREPARATION prep;
    FILE *stream;
    bool done = false;

  protected:
    pxtnERR err;
    char err_str[512];

  public:
    PXToneInstance(PXTone *parent, FILE *px_stream);
    ~PXToneInstance();

    virtual unsigned int getAudio(float *a_buffer, unsigned int a_samples, unsigned int sample_size);
    virtual bool hasEnded();

    unsigned int get_audio(float *a_buffer, unsigned int a_samples, unsigned int sample_size);
    bool has_ended();
    virtual result seek(time a_seconds, float *m_scratch, unsigned int m_scratch_size);
    virtual result rewind();
};

} // namespace SoLoud
