#include "kgmGuiSelect.h"

kgmGuiSelect::kgmGuiSelect()
  :kgmGui()
{
  m_text = new kgmGuiText(this, 0, 0, 0, 0);
  m_list = new kgmGuiList(this, 0, 0, 0, 0);

  slotSelect.connect(this, &kgmGuiSelect::onSelect, &m_list->sigSelect);

  m_point = true;
}

kgmGuiSelect::kgmGuiSelect(kgmGui *par, int x, int y, int w, int h)
  :kgmGui(par, x, y, w, h)
{
  m_text = new kgmGuiText(this, 0, 0, w, h);
  m_list = new kgmGuiList(this, 0, h, w, 2 * h);
  m_text->show();
  m_list->hide();

  slotSelect.connect(this, &kgmGuiSelect::onSelect, &m_list->sigSelect);

  m_text->setEditable(false);

  m_point = true;
}

void kgmGuiSelect::add(kgmString s)
{
  m_list->addItem(s);
}

void kgmGuiSelect::select(kgmString item)
{
  if(m_list->hasItem(item))
    m_text->setText(item);
}

kgmString kgmGuiSelect::getText()
{
  kgmString result;

  if(m_text)
    result = m_text->getText();

  return result;
}

void kgmGuiSelect::onSelect(u32 i)
{
  m_text->setText(m_list->getItem(i));

  sigSelect(m_text->getText());

  if(m_list->visible())
  {
    m_list->hide();
    m_rect.h -= m_list->m_rect.height();
  }
}

void kgmGuiSelect::onMsLeftDown(int k, int x, int y)
{
  kgmGui::onMsLeftDown(k, x, y);

  if(m_text->m_rect.inside(x, y))
  {
    if(!m_list->visible())
    {
      m_list->show();
      m_rect.h += m_list->m_rect.height();
    }
    else
    {
      m_list->hide();
      m_rect.h -= m_list->m_rect.height();
    }
  }
}
