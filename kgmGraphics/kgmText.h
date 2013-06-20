#ifndef KGMTEXT_H
#define KGMTEXT_H

class kgmText: public kgmObject
{
public:
  kgmString  m_text;
  uRect      m_rect;
  u32        m_color;

public:
  kgmText()
  {
    m_text  = "";
    m_rect  = uRect(10, 10, 300, 100);
    m_color = 0xffffffff;
  }

  virtual ~kgmText()
  {
    m_text.clear();
  }
};

#endif // KGMTEXT_H
