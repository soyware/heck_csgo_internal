#pragma once

namespace Hooks
{

	typedef void(__thiscall* PaintTraverseFn) (IVPanel*, unsigned int, bool, bool);
	PaintTraverseFn oPaintTraverse;

	void __fastcall PaintTraverse(IVPanel* _this, void* _edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
	{
		if (Settings::Visuals::NoSniperScope.GetInt())
		{
			static unsigned int zoomPanel = 0;

			if (!zoomPanel)
			{
				if (!strcmp("HudZoom", _this->GetName(vguiPanel)))
					zoomPanel = vguiPanel;
			}
			else if (zoomPanel == vguiPanel)
				return;
		}

		oPaintTraverse(_this, vguiPanel, forceRepaint, allowForce);

		static unsigned int focusOverlayPanel = 0;

		if (!focusOverlayPanel)
		{
			if (!strcmp("FocusOverlayPanel", _this->GetName(vguiPanel)))
				focusOverlayPanel = vguiPanel;
		}
		else if (focusOverlayPanel == vguiPanel)
		{
			I::Surface->PushMakeCurrent(vguiPanel, false);
			Features::Visuals::Paint();
			I::Surface->PopMakeCurrent(vguiPanel);
		}
	}

}