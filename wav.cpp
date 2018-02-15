#include "wav.hpp"
#include <fstream>

using namespace std;

bool write_wav(Signal & sound, const std::string & filename)
{
	int8_t  ChunkID[4];        // "RIFF"
	int32_t  ChunkSize;         // 4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
	int8_t   Format[4];         // "WAVE"
	int32_t  Subchunk1Size = 16;    // 16
	int16_t  AudioFormat = 1;      // 1
	int16_t  NumChannels = 1;      // Mono = 1
	int32_t  SampleRate = 44100;       // samples per second, 44100
	int32_t  ByteRate;         // SampleRate * NumChannels * BitsPerSample/8
	int16_t  BlockAlign;       // NumChannels * BitsPerSample/8
	int16_t  BitsPerSample = 16;    // 8 bits = 8, 16 bits = 16
	int8_t   Subchunk2ID[4];   // "data"
	int32_t  Subchunk2Size;    // NumSamples * NumChannels * BitsPerSample/8
							   //int16_t  Data[NUMSAMPLES]; // the Pulse Code Modulated data

							   //set Chunk ID
	ChunkID[3] = 0x46;
	ChunkID[2] = 0x46;
	ChunkID[1] = 0x49;
	ChunkID[0] = 0x52;


	//set Format
	Format[3] = 0x45;
	Format[2] = 0x56;
	Format[1] = 0x41;
	Format[0] = 0x57;

	//setByteRate
	ByteRate = SampleRate*NumChannels*BitsPerSample / 8;
	//set BlockAlign
	BlockAlign = NumChannels * BitsPerSample / 8;

	//setSubchunk2ID
	Subchunk2ID[3] = 0x61;
	Subchunk2ID[2] = 0x74;
	Subchunk2ID[1] = 0x61;
	Subchunk2ID[0] = 0x64;

	size_t NumSamples = sound.size();
	//set Subchunk2Size
	Subchunk2Size = NumSamples * NumChannels * BitsPerSample / 8;

	//set Chunk size
	ChunkSize = 4 + (8 + Subchunk1Size) + (8 + Subchunk2Size);

	int32_t Subchunk1ID = 0x20746d66;
	string output = filename + "-0.wav";
	std::ofstream outstream(output, std::ios::binary);

	outstream.write(reinterpret_cast<std::fstream::char_type*>(&ChunkID), sizeof ChunkID);
	outstream.write(reinterpret_cast<std::fstream::char_type*>(&ChunkSize), sizeof ChunkSize);
	outstream.write(reinterpret_cast<std::fstream::char_type*>(&Format), sizeof Format);
	outstream.write(reinterpret_cast<std::fstream::char_type*>(&Subchunk1ID), sizeof Subchunk1ID);
	outstream.write(reinterpret_cast<std::fstream::char_type*>(&Subchunk1Size), sizeof Subchunk1Size);
	outstream.write(reinterpret_cast<std::fstream::char_type*>(&AudioFormat), sizeof AudioFormat);
	outstream.write(reinterpret_cast<std::fstream::char_type*>(&NumChannels), sizeof NumChannels);
	outstream.write(reinterpret_cast<std::fstream::char_type*>(&SampleRate), sizeof SampleRate);
	outstream.write(reinterpret_cast<std::fstream::char_type*>(&ByteRate), sizeof ByteRate);
	outstream.write(reinterpret_cast<std::fstream::char_type*>(&BlockAlign), sizeof BlockAlign);
	outstream.write(reinterpret_cast<std::fstream::char_type*>(&BitsPerSample), sizeof BitsPerSample);
	outstream.write(reinterpret_cast<std::fstream::char_type*>(&Subchunk2ID), sizeof Subchunk2ID);
	outstream.write(reinterpret_cast<std::fstream::char_type*>(&Subchunk2Size), sizeof Subchunk2Size);

	for (unsigned int i = 0; i < sound.size(); i++)
	{
		int16_t s = (int16_t)sound[i];
		outstream.write(reinterpret_cast<std::fstream::char_type*>(&s), sizeof s);
		
	}

	return true;
}
