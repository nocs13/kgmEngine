#include "GuiRender.h"
#include "Draw.h"

#include "../kgmGraphics.h"
#include "../kgmGuiLabel.h"
#include "../kgmGuiCheck.h"

GuiRender::GuiRender(kgmGraphics* gr)
{
  this->gr = gr;

  mi.identity();
}

void GuiRender::render()
{
  gr->gc->gcSetShader(null);
  gr->gc->gcDepth(false, 0, 0);
  gr->gc2DMode();

  gr->setWorldMatrix(mi);

  gr->render(gr->m_shaders[kgmGraphics::kgmShader_TypeGui]);

  for(int i = gr->m_guis.size(); i > 0; i--)
  {
    kgmGui* gui = gr->m_guis[i - 1];

    if(!gui)
    {
      gr->m_guis.erase(i - 1);
    }
    else if(gui->visible())
    {
      render(gui);
    }
  }

  gr->render((kgmShader*)null);

  gr->gc3DMode();
}

void GuiRender::render(kgmGui* gui)
{
  kgmGui::Rect rect;
  kgmString    text;

  if(!gui)
    return;

  gui->getRect(rect, true);

  text = gui->getText();

  if(gui->alpha())
    gr->gc->gcBlend(true, gcblend_srcalpha, gcblend_srcialpha);

  if(gui->isClass(kgmGuiButton::cClass()))
  {
    u32 fwidth = (u32)((float)rect.w / (float)(text.length() + 1));
    u32 fheight = (u32)((float)rect.h * (float)0.75f);
    u32 tlen = text.length();
    u32 fw = (tlen) * fwidth;
    u32 fh = fheight;
    kgmGui::Rect tClip = rect;

    tClip.x = rect.x + rect.w / 2 - fw / 2;
    tClip.y = rect.y + rect.h / 2 - fh / 2;
    tClip.w = fw;
    tClip.h = fh;

    switch(((kgmGuiButton*)gui)->getState())
    {
    case kgmGuiButton::StateFocus:
      gcDrawRect(gr->gc, rect, gr->gui_style->sbutton.ac_color, gr->gui_style->gui_image);
      break;
    case kgmGuiButton::StateClick:
      gcDrawRect(gr->gc, rect, gr->gui_style->sbutton.fg_color, gr->gui_style->gui_image);
      break;
    case kgmGuiButton::StateNone:
    default:
      gcDrawRect(gr->gc, rect, gr->gui_style->sbutton.bg_color, gr->gui_style->gui_image);
    }

    if(text.length() > 0)
      gcDrawText(gr->gc, gr->gui_style->gui_font, fwidth, fheight, gr->gui_style->sbutton.tx_color, tClip, text);
  }
  else if(gui->isClass(kgmGuiScroll::cClass()))
  {
    kgmGuiScroll* gscroll = (kgmGuiScroll*)gui;
    kgmGui::Rect  srect = gscroll->getScrollerRect();

    srect = gscroll->toAbsolute(srect);

    gcDrawRect(gr->gc, rect, gr->gui_style->sscroll.bg_color, gr->gui_style->gui_image);
    gcDrawRect(gr->gc, srect, gr->gui_style->sscroll.fg_color, gr->gui_style->gui_image);
  }
  else if(gui->isClass(kgmGuiList::cClass()))
  {
    kgmGuiList* glist = (kgmGuiList*)gui;

    u32 item_cnt = ((kgmGuiList*)gui)->m_items.size();

    kgmGui::Rect srect = glist->getItemRect(glist->getFirstVisibleItem());
    srect = glist->toAbsolute(srect);

    //Draw Main Rect
    gcDrawRect(gr->gc, rect, gr->gui_style->slist.bg_color, gr->gui_style->gui_image);

    //Draw Items Rects
    for(int i = glist->getFirstVisibleItem();
        i < (glist->getFirstVisibleItem() + glist->getVisibleItemsCount());
        i++)
    {
      if(i >= item_cnt)
        break;

      kgmString item;

      item = ((kgmGuiList*)gui)->m_items[i];

      kgmGui::Rect frect;

      frect = glist->getItemRect(i);
      frect = glist->toAbsolute(frect);

      if(i == glist->getSel())
      {
        gcDrawRect(gr->gc, frect, gr->gui_style->slist.fg_color, gr->gui_style->gui_image);
      }
      else if (i == glist->getPnt())
      {
        gcDrawRect(gr->gc, frect, gr->gui_style->slist.ac_color, gr->gui_style->gui_image);
      }

      if(rect.inside(frect))
      {
        gcDrawText(gr->gc, gr->gui_style->gui_font, frect.height() / 2, frect.height(),
                   gr->gui_style->slist.tx_color, frect, item);
      }
    }

    if(glist->m_scroll && glist->m_scroll->visible())
    {
      render(glist->m_scroll);
    }
  }
  else if(gui->isClass(kgmGuiText::cClass()))
  {
    kgmGuiText* gtext = (kgmGuiText*)gui;

    if(gui->m_hasMouse )
    {
      gcDrawRect(gr->gc, rect, gr->gui_style->stext.fg_color, gr->gui_style->gui_image);
    }
    else
    {
      gcDrawRect(gr->gc, rect, gr->gui_style->stext.bg_color, gr->gui_style->gui_image);
    }

    if(text.length() > 0)
    {
      if(!gtext->isReadOnly() && gui->getFocus() == gui)
      {
        u32 i = gtext->getCursor();
        kgmGui::Rect rc(rect.x + 1 + (u32)(0.5 * gr->gui_style->stext.ft_size) * i, rect.y + 1,
                        (u32)(0.5 * gr->gui_style->stext.ft_size - 1), gr->gui_style->stext.ft_size);

        if(rect.inside(rc))
          gcDrawRect(gr->gc, rc, gr->gui_style->stext.cr_color, null);
      }

      kgmString c_text;
      u32       c_len = rect.width() / (u32)(0.5 * gr->gui_style->stext.ft_size);
      s32       c_idx = gtext->getIndex() - gtext->getCursor();

      if (c_idx < 0)
        c_idx = 0;

      if(c_idx + c_len > text.length())
        c_len = text.length() - c_idx;

      c_text.alloc(c_len);

      for(u32 i = 0; i < c_len; i++)
      {
        if(c_idx + i >= text.length())
          break;

        c_text.data()[i] = text.data()[c_idx + i];
      }

      gcDrawText(gr->gc, gr->gui_style->gui_font, (u32)(0.5 * gr->gui_style->stext.ft_size),
                 gr->gui_style->stext.ft_size, gr->gui_style->stext.tx_color, rect, c_text);
    }
  }
  else if(gui->isClass(kgmGuiMenu::cClass()))
  {
    kgmGuiMenu* menu = (kgmGuiMenu*)gui;

    if(menu->getItem())
      renderGuiMenuItem(menu, menu->getItem());
  }
  else if(gui->isClass(kgmGuiTab::cClass()))
  {
    //render((kgmGuiTab*)gui);
  }
  else if(gui->isClass(kgmGuiProgress::cClass()))
  {
    //render((kgmGuiProgress*)gui);
  }
  else if(gui->isClass(kgmGuiLabel::cClass()))
  {
    u32 fwidth = (u32)((float)rect.w / (float)(text.length() + 1));
    u32 fheight = (u32)((float)rect.h * (float)0.75f);
    u32 tlen = text.length();
    u32 fw = (tlen) * fwidth;
    u32 fh = fheight;
    kgmGui::Rect tClip = rect;

    tClip.x = rect.x + rect.w / 2 - fw / 2;
    tClip.y = rect.y + rect.h / 2 - fh / 2;
    tClip.w = fw;
    tClip.h = fh;

    fwidth = ((kgmGuiLabel*)gui)->getFontWidth();
    fheight = ((kgmGuiLabel*)gui)->getFontHeight();

    gcDrawRect(gr->gc, rect, gr->gui_style->slabel.bg_color, gr->gui_style->gui_image);

    if(text.length() > 0)
      gcDrawText(gr->gc, gr->gui_style->gui_font, fwidth, fheight, gr->gui_style->sbutton.tx_color, tClip, text);
  }
  else if(gui->isClass(kgmGuiCheck::cClass()))
  {
    u32 fwidth = (u32)((float)(rect.w - 25) / (float)(text.length() + 1));
    u32 fheight = (u32)((float)rect.h * (float)0.75f);
    u32 tlen = text.length();
    u32 fw = (tlen) * fwidth;
    u32 fh = fheight;
    kgmGui::Rect tClip = rect;

    tClip.x = rect.x + rect.w / 2 - fw / 2;
    tClip.y = rect.y + rect.h / 2 - fh / 2;
    tClip.w = fw;
    tClip.h = fh;

    kgmGui::Rect btClip = rect;
    kgmGui::Rect txClip = rect;
    kgmGui::Rect chClip = rect;

    btClip.x += 2;
    btClip.y += 2;
    btClip.w = 20;
    btClip.h = rect.h - 4;

    chClip = btClip;
    chClip.x += 3;
    chClip.y += 3;
    chClip.w -= 6;
    chClip.h -= 6;

    txClip.x += 25;
    txClip.w -= 25;

    fwidth = ((kgmGuiCheck*)gui)->getFontWidth();
    fheight = ((kgmGuiCheck*)gui)->getFontHeight();

    switch(((kgmGuiCheck*)gui)->getState())
    {
    case kgmGuiCheck::StateFocus:
      gcDrawRect(gr->gc, rect, gr->gui_style->scheck.ac_color, gr->gui_style->gui_image);
      break;
    case kgmGuiCheck::StateNone:
    default:
      gcDrawRect(gr->gc, rect, gr->gui_style->scheck.bg_color, gr->gui_style->gui_image);
    }

    gcDrawRect(gr->gc, btClip, gr->gui_style->scheck.bg_check, gr->gui_style->gui_image);

    if(((kgmGuiCheck*)gui)->isCheck())
      gcDrawRect(gr->gc, chClip, gr->gui_style->scheck.fg_check, gr->gui_style->gui_image);

    if(text.length() > 0)
      gcDrawText(gr->gc, gr->gui_style->gui_font, fwidth, fheight, gr->gui_style->sbutton.tx_color, txClip, text);
  }
  else if(gui->isClass(kgmGui::cClass()))
  {
    if(gui->m_hasMouse )
    {
      gcDrawRect(gr->gc, rect, gr->gui_style->sgui.fg_color, gr->gui_style->gui_image);
    }
    else
    {
      gcDrawRect(gr->gc, rect, gr->gui_style->sgui.bg_color, gr->gui_style->gui_image);
    }
  }

  if(gui->alpha())
    gr->gc->gcBlend(false, gcblend_zero, gcblend_zero);

  for(int i = 0; i < gui->m_childs.length(); i++)
  {
    if(gui->m_childs[i]->m_view)
      render(gui->m_childs[i]);
  }
}

void GuiRender::renderGuiMenuItem(kgmGui* m, void *i)
{
  kgmGuiMenu* menu = static_cast<kgmGuiMenu*> (m);

  kgmGuiMenu::Item* item = static_cast<kgmGuiMenu::Item*> (i);

  kgmGui::Rect prect;

  menu->getRect(prect, true);

  for(int i = 0; i < item->getItemsCount(); i++)
  {
    kgmGuiMenu::Item* citem = item->getItem(i);

    kgmGui::Rect rc = item->getRect(i);

    rc.x += prect.x;
    rc.y += prect.y;

    kgmGuiMenu::Item* choose = menu->getChoose();

    if(citem == choose)
    {
      gcDrawRect(gr->gc, rc, gr->gui_style->smenu.ac_color, gr->gui_style->gui_image);
    }
    else if(item->getSelected() == i)
    {
      if(citem->getType() == kgmGuiMenu::Item::TypeMenu)
        renderGuiMenuItem(menu, citem);

      gcDrawRect(gr->gc, rc, gr->gui_style->smenu.fg_color, gr->gui_style->gui_image);
    }
    else
    {
      gcDrawRect(gr->gc, rc, gr->gui_style->smenu.bg_color, gr->gui_style->gui_image);
    }

    kgmString title = citem->getTitle();

    gcDrawText(gr->gc, gr->font, 8, 19, gr->gui_style->smenu.tx_color, rc, title);
  }
}
