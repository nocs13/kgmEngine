#include "kgmGuiSelect.h"

KGMOBJECT_IMPLEMENT(kgmGuiSelect, kgmGui)

kgmGuiSelect::kgmGuiSelect()
  :kgmGui()
{
  m_text = new kgmGuiText(this, 0, 0, 0, 0);
  m_list = new kgmGuiList(this, 0, 0, 0, 0);
}

kgmGuiSelect::kgmGuiSelect(kgmGui *par, int x, int y, int w, int h)
  :kgmGui(par, x, y, w, h)
{
  m_text = new kgmGuiText(this, 0, 0, w, h);
  m_list = new kgmGuiList(this, 0, h, w, 2 * h);
  m_text->show();
  m_list->hide();

  m_text->setEditable(false);
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

  if(m_list->visible())
    m_list->hide();
}

void kgmGuiSelect::onMsLeftDown(int k, int x, int y)
{
  kgmGui::onMsLeftDown(k, x, y);

  if(m_list && !m_list->visible())
    m_list->show();
}
