//=========================================================================
// FILENAME	: tagutils-mp3.h
// DESCRIPTION	: MP3 metadata reader
//=========================================================================
// Copyright (c) 2008- NETGEAR, Inc. All Rights Reserved.
//=========================================================================

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <id3tag.h>

#define XING_FRAMES  0x01
#define XING_BYTES   0x02
#define XING_TOC     0x04
#define XING_QUALITY 0x08

struct mp3_frameinfo {
  int layer;					// 1,2,3
  int bitrate;					// unit=kbps
  int samplerate;				// samp/sec
  int stereo;					// flag

  int frame_length;				// bytes
  int crc_protected;				// flag
  int samples_per_frame;			// calculated
  int padding;					// flag
  
  // Xing header
  int xing_offset;
  int xing_frames;
  int xing_bytes;
  int xing_quality;

  int frame_offset;

  short mpeg_version;
  short id3_version;

  int is_valid;
};

static int _get_mp3tags(char *file, HV *tags);
static int _get_mp3fileinfo(char *file, HV *info);
static int _decode_mp3_frame(unsigned char *frame, struct mp3_frameinfo *pfi);

// bitrate_tbl[layer_index][bitrate_index]
static int bitrate_tbl[5][16] = {
  { 0,32,64,96,128,160,192,224,256,288,320,352,384,416,448,0 }, /* MPEG1, L1 */
  { 0,32,48,56,64,80,96,112,128,160,192,224,256,320,384,0 },    /* MPEG1, L2 */
  { 0,32,40,48,56,64,80,96,112,128,160,192,224,256,320,0 },     /* MPEG1, L3 */
  { 0,32,48,56,64,80,96,112,128,144,160,176,192,224,256,0 },    /* MPEG2/2.5, L1 */
  { 0,8,16,24,32,40,48,56,64,80,96,112,128,144,160,0 }          /* MPEG2/2.5, L2/L3 */
};

// sample_rate[sample_index][samplerate_index]
static int sample_rate_tbl[3][4] = {
  { 44100, 48000, 32000, 0 },  /* MPEG 1 */
  { 22050, 24000, 16000, 0 },  /* MPEG 2 */
  { 11025, 12000, 8000, 0 }    /* MPEG 2.5 */
};
