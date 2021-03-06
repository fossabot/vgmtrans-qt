#pragma once
#include "Loader.h"
#include "PSFFile.h"

class NDS2SFLoader : public VGMLoader {
 public:
  NDS2SFLoader(void);

 public:
  virtual ~NDS2SFLoader(void);

  virtual PostLoadCommand Apply(RawFile *theFile);
  const wchar_t *psf_read_exe(RawFile *file, unsigned char *&exebuffer,
                              size_t &exebuffersize);

 private:
  const wchar_t *load_psf_libs(PSFFile &psf, RawFile *file,
                               unsigned char *&exebuffer,
                               size_t &exebuffersize);
};
