// -*- Mode: c++ -*-
/** TextSubtitles
 *  Copyright (c) 2006 by Pekka Jääskeläinen
 *  Distributed as part of MythTV under GPL v2 and later.
 */

#ifndef TEXT_SUBTITLE_PARSER_H
#define TEXT_SUBTITLE_PARSER_H

// POSIX headers
#include <stdint.h>

// C++ headers
#include <vector>
using namespace std;

// Qt headers
#include <QStringList>

class text_subtitle_t
{
  public:
    text_subtitle_t(long start_, long end_) : start(start_), end(end_) {}
    text_subtitle_t() : start(0), end(0) {}
    text_subtitle_t(const text_subtitle_t &other) :
        start(other.start), end(other.end),
        textLines(other.textLines)
    {
        textLines.detach();
    }

  public:
    uint64_t    start;      ///< Starting time in msec or starting frame
    uint64_t    end;        ///< Ending time in msec or ending frame
    QStringList textLines;
};

typedef vector<text_subtitle_t> TextSubtitleList;

class TextSubtitles
{
  public:
    TextSubtitles() : m_frameBasedTiming(false)
    {
        m_lastReturnedSubtitle.start = 0;
        m_lastReturnedSubtitle.end   = 0;
    }

    virtual ~TextSubtitles() {}

    bool HasSubtitleChanged(uint64_t timecode) const;
    QStringList GetSubtitles(uint64_t timecode) const;

    /** \fn TextSubtitles::IsFrameBasedTiming(void) const
     *  \brief Returns true in case the subtitle timing data is frame-based.
     *
     *  If the timing is frame-based, the client should use frame counts as
     *  timecodes for the HasSubtitleChanged() and GetSubtitles() methods,
     *  otherwise the timecode is milliseconds from the video start.
     */
    bool IsFrameBasedTiming(void) const
        { return m_frameBasedTiming; }

    void SetFrameBasedTiming(bool frameBasedTiming)
        { m_frameBasedTiming = frameBasedTiming; }

    void AddSubtitle(const text_subtitle_t& newSub);
    void Clear(void);

    uint GetSubtitleCount(void) const
        { return m_subtitles.size(); }

    void Lock(void)   { m_lock.lock();   }
    void Unlock(void) { m_lock.unlock(); }

  private:
    TextSubtitleList          m_subtitles;
    mutable text_subtitle_t   m_lastReturnedSubtitle;
    bool                      m_frameBasedTiming;
    QMutex                    m_lock;
};

class TextSubtitleParser
{
  public:
    static bool LoadSubtitles(QString fileName, TextSubtitles &target);
};

#endif
