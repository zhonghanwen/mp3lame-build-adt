#include <stdio.h>
#include <stdlib.h>
#include <jni.h>
#include "lame.h"

#define LOG_TAG "LAME ENCODER"
#define LOGD(format, args...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, format, ##args);
#define BUFFER_SIZE 8192
#define be_short(s) ((short) ((unsigned short) (s) << 8) | ((unsigned short) (s) >> 8))

lame_t lame;

int read_samples(FILE *input_file, short *input) {
	int nb_read;
	nb_read = fread(input, 1, sizeof(short), input_file) / sizeof(short);

	int i = 0;
	while (i < nb_read) {
		input[i] = be_short(input[i]);
		i++;
	}

	return nb_read;
}

void Java_com_skyhi_audio_LameUtil_initEncoder(JNIEnv *env,
		jobject jobj, jint in_num_channels, jint in_samplerate, jint in_brate,
		jint in_mode, jint in_quality) {
	lame = lame_init();

	lame_set_num_channels(lame, in_num_channels);
	lame_set_in_samplerate(lame, in_samplerate);
	lame_set_brate(lame, in_brate);
	lame_set_mode(lame, in_mode);
	lame_set_quality(lame, in_quality);
	lame_init_params(lame);
}

void Java_com_skyhi_audio_LameUtil_destroyEncoder(
		JNIEnv *env, jobject jobj) {
	lame_close(lame);
}

void Java_com_skyhi_audio_LameUtil_encodeFile(JNIEnv *env,
		jobject jobj, jstring in_source_path, jstring in_target_path) {
	const char *source_path, *target_path;
	source_path = (*env)->GetStringUTFChars(env, in_source_path, NULL);
	target_path = (*env)->GetStringUTFChars(env, in_target_path, NULL);

	FILE *input_file, *output_file;
	input_file = fopen(source_path, "rb");
	output_file = fopen(target_path, "wb");

	short input[BUFFER_SIZE];
	char output[BUFFER_SIZE];
	int nb_read = 0;
	int nb_write = 0;
	int nb_total = 0;

	while (nb_read = read_samples(input_file, input)) {
		nb_write = lame_encode_buffer(lame, input, input, nb_read, output,
				BUFFER_SIZE);
		fwrite(output, nb_write, 1, output_file);
		nb_total += nb_write;
	}

	nb_write = lame_encode_flush(lame, output, BUFFER_SIZE);
	fwrite(output, nb_write, 1, output_file);

	fclose(input_file);
	fclose(output_file);
}
