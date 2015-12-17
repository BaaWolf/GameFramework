
#ifndef FFT_H
#define FFT_H


extern void init_fft(int length, int sampleRate);
extern void done_fft();
extern float find_pitch(unsigned char* buffer, float minFreq, float maxFreq);

#endif  // FFT_H
