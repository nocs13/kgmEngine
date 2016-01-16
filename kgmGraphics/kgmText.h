#ifndef KGMTEXT_H
#define KGMTEXT_H

class kgmText: public kgmObject
{
public:
  kgmString  m_text;
  uRect      m_rect;
  u32        m_size;
  u32        m_color;

public:
  kgmText()
  {
    m_text  = "";
    m_rect  = uRect(10, 10, 300, 100);
    m_size  = 15;
    m_color = 0xffffffff;
  }

  ~kgmText()
  {
    m_text.clear();
  }

  uRect rect() const
  {
    return m_rect;
  }

  void rect(uRect r)
  {
    m_rect = r;
  }
};

#endif // KGMTEXT_H
