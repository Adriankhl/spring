#include "Button.h"

#include "Gui.h"
#include "LogOutput.h"
#include "Rendering/glFont.h"

namespace agui
{

Button::Button(const std::string& _label, GuiElement* _parent) : GuiElement(_parent)
{
	Label(_label);
}

void Button::Label(const std::string& _label)
{
	label = _label;
	clicked = false;
	hovered = false;
}

boost::signals::connection Button::ClickHandler(SignalType::slot_function_type handler)
{
	return sig.connect(handler);
}

void Button::DrawSelf()
{
	glColor4f(0.8f,0.8f,0.8f, 0.8f);
	
	DrawBox(GL_QUADS);
	
	glColor4f(1,1,1,0.1f);
	if (clicked) {
		glBlendFunc(GL_ONE, GL_ONE); // additive blending
		glColor4f(0.2f,0,0,1);
		DrawBox(GL_QUADS);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1,0,0,0.5f);
		glLineWidth(1.49f);
		DrawBox(GL_LINE_LOOP);
		glLineWidth(1.0f);
	}
	else if (hovered)
	{
		glBlendFunc(GL_ONE, GL_ONE); // additive blending
		glColor4f(0,0,0.2f,1);
		DrawBox(GL_QUADS);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1,1,1,0.5f);
		glLineWidth(1.49f);
		DrawBox(GL_LINE_LOOP);
		glLineWidth(1.0f);
	}
		
	font->SetTextColor(); //default
	font->SetOutlineColor(0.0f, 0.0f, 0.0f, 1.0f);
	font->glPrint(pos[0]+0.05, pos[1]+size[1]/2, 1.0, FONT_VCENTER | FONT_SHADOW | FONT_SCALE | FONT_NORM, label);
}

bool Button::HandleEventSelf(const SDL_Event& ev)
{
	switch (ev.type) {
		case SDL_MOUSEBUTTONDOWN: {
			if (MouseOver(ev.button.x, ev.button.y) && gui->MouseOverElement(GetRoot(), ev.motion.x, ev.motion.y))
			{
				clicked = true;
			};
			break;
		}
		case SDL_MOUSEBUTTONUP: {
			if (MouseOver(ev.button.x, ev.button.y) && clicked)
			{
				if (!sig.empty())
				{
					sig();
				}
				else
				{
					LogObject() << "Button " << label << " clicked without callback";
				}
				clicked = false;
				return true;
			}
		}
		case SDL_MOUSEMOTION: {
			if (MouseOver(ev.motion.x, ev.motion.y) && gui->MouseOverElement(GetRoot(), ev.motion.x, ev.motion.y))
			{
				hovered = true;
			}
			else
			{
				hovered = false;
				clicked = false;
			}
			break;
		}
	}
	return false;
}

}
