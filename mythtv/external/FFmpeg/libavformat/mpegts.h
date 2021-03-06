/*
 * MPEG2 transport stream defines
 * Copyright (c) 2003 Fabrice Bellard
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef AVFORMAT_MPEGTS_H
#define AVFORMAT_MPEGTS_H

#include "avformat.h"

#define TS_FEC_PACKET_SIZE 204
#define TS_DVHS_PACKET_SIZE 192
#define TS_PACKET_SIZE 188
#define TS_MAX_PACKET_SIZE 204

#define NB_PID_MAX 8192
#define MAX_SECTION_SIZE 4096

/* pids */
#define PAT_PID                 0x0000
#define SDT_PID                 0x0011

/* table ids */
#define PAT_TID   0x00
#define PMT_TID   0x02
#define SDT_TID   0x42

#define DVB_CAROUSEL_ID             0x13
#define DVB_VBI_DATA_ID             0x45
#define DVB_VBI_TELETEXT_ID         0x46
#define DVB_TELETEXT_ID             0x56
#define DVB_SUBT_DESCID             0x59
#define DVB_BROADCAST_ID            0x66
#define DVB_DATA_STREAM             0x52
#define STREAM_TYPE_VIDEO_MPEG1     0x01
#define STREAM_TYPE_VIDEO_MPEG2     0x02
#define STREAM_TYPE_AUDIO_MPEG1     0x03
#define STREAM_TYPE_AUDIO_MPEG2     0x04
#define STREAM_TYPE_PRIVATE_SECTION 0x05
#define STREAM_TYPE_PRIVATE_DATA    0x06
#define STREAM_TYPE_DSMCC_B         0x0b
#define STREAM_TYPE_AUDIO_AAC       0x0f
#define STREAM_TYPE_AUDIO_AAC_LATM  0x11
#define STREAM_TYPE_VIDEO_MPEG4     0x10
#define STREAM_TYPE_VIDEO_H264      0x1b
#define STREAM_TYPE_VIDEO_VC1       0xea
#define STREAM_TYPE_VIDEO_DIRAC     0xd1

#define STREAM_TYPE_AUDIO_AC3       0x81
#define STREAM_TYPE_AUDIO_DTS       0x8a
#define STREAM_TYPE_AUDIO_HDMV_AC3_PLUS      0x84
#define STREAM_TYPE_AUDIO_HDMV_AC3_TRUE_HD   0x83
#define STREAM_TYPE_AUDIO_HDMV_DTS           0x82
#define STREAM_TYPE_AUDIO_HDMV_DTS_HD        0x85
#define STREAM_TYPE_AUDIO_HDMV_DTS_HD_MASTER 0x86

#define STREAM_TYPE_SUBTITLE_DVB    0x100
#define STREAM_TYPE_VBI_DVB         0x101

typedef struct MpegTSContext MpegTSContext;

void mpegts_remove_stream(MpegTSContext *ts, int pid);

MpegTSContext *ff_mpegts_parse_open(AVFormatContext *s);
int ff_mpegts_parse_packet(MpegTSContext *ts, AVPacket *pkt,
                           const uint8_t *buf, int len);
void ff_mpegts_parse_close(MpegTSContext *ts);

typedef struct
{
    char language[4];
    int comp_page;
    int anc_page;
    int sub_id;
    int txt_type;
    int vbi_data;
    /* DSMCC data */
    int data_id;
    int carousel_id;
    int component_tag;
    unsigned int codec_tag;
} dvb_caption_info_t;

typedef struct
{
    int pid;
    int type;
    enum CodecID       codec_id;
    enum CodecType     codec_type;
    dvb_caption_info_t dvbci;
} pmt_entry_t;

/**
 * Parse an MPEG-2 descriptor
 * @param[in] fc                    Format context (used for logging only)
 * @param st                        Stream
 * @param stream_type               STREAM_TYPE_xxx
 * @param pp                        Descriptor buffer pointer
 * @param desc_list_end             End of buffer
 * @param mp4_dec_config_descr_len  Length of 'mp4_dec_config_descr', or zero if not present
 * @param mp4_es_id
 * @param pid
 * @param mp4_dec_config_descr
 * @return <0 to stop processing
 */
int ff_parse_mpeg2_descriptor(AVFormatContext *fc, pmt_entry_t *item, int stream_type,
                              const uint8_t **pp, const uint8_t *desc_list_end,
                              int mp4_dec_config_descr_len, int mp4_es_id, int pid,
                              uint8_t *mp4_dec_config_descr, 
                              dvb_caption_info_t *dvbci);

#endif /* AVFORMAT_MPEGTS_H */
