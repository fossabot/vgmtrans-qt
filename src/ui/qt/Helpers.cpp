#include "Helpers.h"

QIcon iconForFileType(FileType filetype) {
  switch (filetype) {
    case FILETYPE_SEQ:
      return QIcon(":/images/music_transcripts-32.png");
    case FILETYPE_INSTRSET:
      return QIcon(":/images/piano-32.png");
    case FILETYPE_SAMPCOLL:
      return QIcon(":/images/audio_wave-32.png");
  }
  return QIcon(":/images/audio_file-32.png");
}

// QColor(174, 138, 190, 255) //purple
// QColor(188, 63, 60, 255) //dark red
// QColor(158, 41, 39, 255) //bright red
// QColor(69, 5, 5, 255) //darker red
// QColor(83, 43, 46, 255) //wine red
// QColor(168, 192, 35, 255) // lighter green
// QColor(50, 89, 61, 255) //jungle green
// QColor(21, 82, 33, 255) //even darker green
// QColor(54 65 53, 255) //super dark green
// QColor(101 156 107, 255) //weird grayish green
// QColor(40 123 222, 255) //blue
// QColor(244 152 16, 255) //orange
// QColor(176 97 0, 255) //darker orange
// QColor(94 83 57, 255) //brownish orange

QColor colorForEventColor(uint8_t eventColor) {
  switch (eventColor) {
    case CLR_UNKNOWN:
      return Qt::black;
    case CLR_UNRECOGNIZED:
      return Qt::red;
    case CLR_HEADER:
      return Qt::lightGray;
    case CLR_MISC:
      return Qt::gray;
    case CLR_MARKER:
      return Qt::gray;
    case CLR_TIMESIG:
      return Qt::green;
    case CLR_TEMPO:
      return Qt::darkGreen;
    case CLR_PROGCHANGE:
      return QColor(244, 152, 16, 255);
    case CLR_TRANSPOSE:
      return Qt::darkMagenta;
    case CLR_PRIORITY:
      return Qt::darkMagenta;
    case CLR_VOLUME:
      return QColor(174, 138, 190, 255);
    case CLR_EXPRESSION:
      return QColor(83, 43, 46, 255);
    case CLR_PAN:
      return QColor(50, 89, 61, 255);
    case CLR_NOTEON:
      return QColor(40, 123, 222, 255);
    case CLR_NOTEOFF:
      return QColor(65, 159, 211, 255);
    case CLR_DURNOTE:
      return QColor(73, 164, 219, 255);
    case CLR_TIE:
      return QColor(117, 109, 220, 255);
    case CLR_REST:
      return QColor(142, 101, 207, 255);
    case CLR_PITCHBEND:
      return Qt::magenta;
    case CLR_PITCHBENDRANGE:
      return Qt::magenta;
    case CLR_MODULATION:
      return QColor(176, 97, 0, 255);
    case CLR_PORTAMENTO:
      return Qt::magenta;
    case CLR_PORTAMENTOTIME:
      return Qt::magenta;
    case CLR_CHANGESTATE:
      return Qt::gray;
    case CLR_ADSR:
      return Qt::gray;
    case CLR_LFO:
      return Qt::gray;
    case CLR_REVERB:
      return Qt::gray;
    case CLR_SUSTAIN:
      return Qt::yellow;
    case CLR_LOOP:
      return QColor(188, 63, 60, 255);
    case CLR_LOOPFOREVER:
      return QColor(188, 63, 60, 255);
    case CLR_TRACKEND:
      return QColor(158, 41, 39, 255);
  }
  return Qt::red;
}

QColor textColorForEventColor(uint8_t eventColor) {
  switch (eventColor) {
    case CLR_UNKNOWN:
    case CLR_VOLUME:
      return Qt::white;
    default:
      return Qt::black;
  }
}

wchar_t* qstringTowchar_t(QString text) {
  wchar_t* cText = new wchar_t[text.length() + 1];
  text.toWCharArray(cText);
  cText[text.length()] = 0;

  return cText;
}
