#pragma once
/*
#include <string>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
*/
typedef void(__thiscall* DrawModelExecuteFn) (IVModelRender*, void*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);

namespace Features
{
	namespace Visuals
	{
		IMaterial* CreateChamsMaterial(bool lit, bool ignoreZ)
		{
			static const char Template[] =
				"%s\n"
				"{\n"
				"	$basetexture vgui\\white\n"
				"	$model 1\n"
				"	$nofog 1\n"
				"	$nodecal 1\n"
				"	$translucent 0\n"
				"	$halflambert 1\n"
				"	$znearer 1\n"
				"	$ignorez %hhu\n"
				"}";

			static unsigned int count = 0;

			char name[17];
			sprintf_s(name, sizeof(name), "mycoolmaterial%u", count);

			++count;

			const char* shader = lit ? "VertexLitGeneric" : "UnlitGeneric";

			char buffer[134];
			sprintf_s(buffer, sizeof(buffer), Template, shader, ignoreZ);

			KeyValues* kv = new KeyValues(shader);
			kv->LoadFromBuffer(name, buffer);

			return I::MaterialSystem->CreateMaterial(shader, kv);
		}

		bool BoundingBoxToScreen(C_BaseEntity* entity, int& outLeft, int& outTop, int& outRight, int& outBottom)
		{
			const vec3 mins(entity->WorldAlignMins());
			const vec3 maxs(entity->WorldAlignMaxs());

			const vec3 corners[] = { vec3(mins.x, mins.y, mins.z),
									vec3(mins.x, maxs.y, mins.z),
									vec3(maxs.x, maxs.y, mins.z),
									vec3(maxs.x, mins.y, mins.z),

									vec3(maxs.x, maxs.y, maxs.z),
									vec3(mins.x, maxs.y, maxs.z),
									vec3(mins.x, mins.y, maxs.z),
									vec3(maxs.x, mins.y, maxs.z) };

			const matrix3x4_t& coordFrame = entity->GetCoordinateFrame();

			outRight = -7680;
			outLeft = 7680;
			outBottom = -4320;
			outTop = 4320;

			for (unsigned int i = 0; i < _countof(corners); ++i)
			{
				vec3 transformed;
				VectorTransform(&corners[i].x, coordFrame, transformed);

				vec3 screen;
				if (!I::DebugOverlay->ScreenPosition(transformed, screen))
					return false;

				if (outRight < (int)screen.x)
					outRight = screen.x;
				else if (outLeft > (int)screen.x)
					outLeft = screen.x;

				if (outBottom < (int)screen.y)
					outBottom = screen.y;
				else if (outTop > (int)screen.y)
					outTop = screen.y;
			}

			if ((7680 <= outRight) || (4320 <= outBottom) || 
				(outLeft <= -7680) || (outTop <= -4320))
				return false;

			return true;
		}

		void DrawBox(int left, int top, int right, int bottom, Color clr, bool fill)
		{
			if (Settings::Visuals::ESP::Style.GetInt()) // Outline
				Draw::OutlinedRect(left - 1, top - 1, right + 1, bottom + 1, Color(0, 0, 0, clr.a));
			else // Shadow
			{
				Draw::Line(left, bottom, right + 1, bottom, Color(0, 0, 0, clr.a));
				Draw::Line(right, top, right, bottom, Color(0, 0, 0, clr.a));
			}

			if (fill)
				Draw::FilledRect(left, top, right, bottom, clr);
			else
				Draw::OutlinedRect(left, top, right, bottom, clr);
		}

		void DrawName(int X, int& nextOverheadY, const int entNum, Color clr, bool printTeamEnemy = false, bool isTeammate = false)
		{
			player_info_t info;
			if (!I::EngineClient->GetPlayerInfo(entNum, &info))
				return;

			wchar_t name[40] = { 0 };
			if (!MultiByteToWideChar(CP_UTF8, 0, info.name, _countof(name) - 1, name, _countof(name)))
				return;

			wchar_t print[48] = { 0 };
			if (printTeamEnemy)
				wcscpy_s(print, _countof(print), (isTeammate ? L"Team: " : L"Enemy: "));

			if (info.fakeplayer)
				wcscat_s(print, _countof(print), L"BOT ");

			wcscat_s(print, _countof(print), name);

			int W;
			I::Surface->GetTextSize(Draw::GetESPFont(), print, W, W);

			const int Y = nextOverheadY - 13;

			Draw::Text(print,
				X - W / 2,
				Y,
				Draw::GetESPFont(),
				clr);

			// a pixel outside of the outline
			nextOverheadY = Y + 1 + !Settings::Visuals::ESP::Style.GetInt();
		}

		// if above is true then Y gets set to a pixel above drawn weapon
		void DrawWeapon(int X, int& Y, bool above, bool icon, bool ammo, C_BaseEntity* entity, int itemIndex, Color clr)
		{
			int _Y = (above ? Y - 13 : Y);

			if (!icon)
			{
				_Y -= !above && !Settings::Visuals::ESP::Style.GetInt();

				wchar_t print[40];
				const wchar_t* weaponName = GetPrintName(itemIndex);

				C_WeaponCSBaseGun* weapon = static_cast<C_WeaponCSBaseGun*>(entity);

				if (ammo && entity->IsBaseCombatWeapon() && weapon->GetClip1() != -1)
				{
					swprintf_s(print, _countof(print), L"%s  %d / %d",
						weaponName, weapon->GetClip1(), weapon->GetPrimaryReserveAmmoCount());
				}
				else
					wcscpy_s(print, _countof(print), weaponName);

				int W; // we only need width (Fn gets height then width)
				I::Surface->GetTextSize(Draw::GetESPFont(), print, W, W);

				Draw::Text(print, X - W / 2, _Y, Draw::GetESPFont(), clr);
			}
			else
			{
				const wchar_t wchIcon = 0x20 + itemIndex;
				const int iconW = I::Surface->GetCharacterWidth(Draw::fontIcons, wchIcon);

				int _X;
				_Y += (above ? -1 : 2);

				C_WeaponCSBaseGun* weapon = static_cast<C_WeaponCSBaseGun*>(entity);

				if (ammo && entity->IsBaseCombatWeapon() && weapon->GetClip1() != -1)
				{
					wchar_t print[16];
					swprintf_s(print, _countof(print), L"%d / %d", weapon->GetClip1(), weapon->GetPrimaryReserveAmmoCount());

					int textW;
					I::Surface->GetTextSize(Draw::GetESPFont(), print, textW, textW);

					_X = X - (textW + iconW + 6) / 2;

					Draw::Text(print, _X + iconW + 6, _Y, Draw::GetESPFont(), clr);
				}
				else
					_X = X - iconW / 2;

				Draw::UnicodeChar(wchIcon, _X, _Y, Draw::fontIcons, clr);
			}

			if (above)
			{
				// a pixel outside of the outline
				if (!icon)
					Y = _Y + 1 + !Settings::Visuals::ESP::Style.GetInt();
				else
					Y = _Y - 1;
			}
		}

		void DrawHorizontalProgressBar(int left, int right, int Y, float cur, float start, float end, Color clrBg)
		{
			const Color clrBar = GetRatioClr(cur, start, end);
			const float progress = std::clamp((cur - start) / (end - start), 0.f, 1.f);
			const int X = round(left + (right - left) * progress);

			Draw::FilledRect(left - Settings::Visuals::ESP::Style.GetInt(),
				Y,
				right + 1,
				Y + 2 + Settings::Visuals::ESP::Style.GetInt(),
				clrBg);

			Draw::Line(left,
				Y + Settings::Visuals::ESP::Style.GetInt(),
				X,
				Y + Settings::Visuals::ESP::Style.GetInt(),
				clrBar);
		}

		void DrawVerticalProgressBar(int X, int top, int bottom, float cur, float start, float end, Color clrBg)
		{
			const Color clrBar = GetRatioClr(cur, start, end);
			const float progress = std::clamp((cur - start) / (end - start), 0.f, 1.f);
			const int Y = bottom - (bottom - top) * progress;

			Draw::FilledRect(X - (Settings::Visuals::ESP::Style.GetInt() ? 2 : 0),
				top - Settings::Visuals::ESP::Style.GetInt(),
				X + 1 + !Settings::Visuals::ESP::Style.GetInt(),
				bottom + 1,
				clrBg);

			Draw::Line(X - Settings::Visuals::ESP::Style.GetInt(),
				Y,
				X - Settings::Visuals::ESP::Style.GetInt(),
				bottom,
				clrBar);
		}

		void RenderLine(const vec3& start, const vec3& end, Color clr, bool ZBuffer)
		{
			typedef void(*Fn)(const vec3&, const vec3&, Color, bool);
			reinterpret_cast<Fn>(Signatures::RenderLine)(start, end, clr, ZBuffer);
		}

		void RenderBoxInternal(const vec3& origin, const vec3& angles, const vec3& mins, const vec3& maxs, Color clr, IMaterial* material, bool insideOut)
		{
			typedef void(*Fn)(const vec3&, const vec3&, const vec3&, const vec3&, Color, IMaterial*, bool);
			reinterpret_cast<Fn>(Signatures::RenderBoxInternal)(origin, angles, mins, maxs, clr, material, insideOut);
		}

		void RenderBox(const vec3& origin, const vec3& angles, const vec3& mins, const vec3& maxs, Color clr, bool ZBuffer, bool insideOut)
		{
			static IMaterial* s_pVertexColor = I::MaterialSystem->FindProceduralMaterial("__utilVertexColor", TEXTURE_GROUP_OTHER, nullptr);
			static IMaterial* s_pVertexColorIgnoreZ = I::MaterialSystem->FindProceduralMaterial("__utilVertexColorIgnoreZ", TEXTURE_GROUP_OTHER, nullptr);

			IMaterial* pMaterial = ZBuffer ? s_pVertexColor : s_pVertexColorIgnoreZ;
			RenderBoxInternal(origin, angles, mins, maxs, clr, pMaterial, insideOut);
		}

		void RenderWireframeBox(const vec3& origin, const vec3& angles, const vec3& mins, const vec3& maxs, Color clr, bool ZBuffer)
		{
			typedef void(*Fn)(const vec3&, const vec3&, const vec3&, const vec3&, Color, bool);
			reinterpret_cast<Fn>(Signatures::RenderWireframeBox)(origin, angles, mins, maxs, clr, ZBuffer);
		}
	}
}

#include "ESP.h"

namespace Features
{
	namespace Visuals
	{
		constexpr int lineSpacing = 16;

		namespace Visuals
		{
			bool GetPlantedC4ClrText(C_CSPlayer* player, C_PlantedC4* entity, Color& outClr, 
				wchar_t* outText = nullptr, size_t outTextWcharCount = 0)
			{
				if (!entity->IsBombTicking())
					return false;

				float timeBlow = entity->GetBlowTime();

				if (entity->GetDefuser() != -1)
				{
					float timeDefuse = entity->GetDefuseCountDown();
					if (timeBlow <= timeDefuse)
					{
						outClr = Color::LightRed();
						if (outText) wcscpy_s(outText, outTextWcharCount, L"C4: Defusing (no time)");
					}
					else
					{
						outClr = Color::LightGreen();
						if (outText) swprintf_s(outText, outTextWcharCount, L"C4: Defusing %.1f", (timeDefuse - G::Globals->curtime));
					}
				}
				else
				{
					float timeLeft = timeBlow - G::Globals->curtime;
					if (timeLeft <= 5.f)
						return false;

					if ((player->GetTeam() == 2) || player->HasDefuser() || (10.f < timeLeft))
					{
						outClr.SetRawColor(Settings::Visuals::Visuals::C4Clr.GetInt());
						if (outText) swprintf_s(outText, outTextWcharCount, L"C4: %.1f", timeLeft);
					}
					else
					{
						outClr = Color::LightRed();
						if (outText) swprintf_s(outText, outTextWcharCount, L"C4: %.1f (no defuser)", timeLeft);
					}
				}
				return true;
			}

			void DrawPlantedC4(int X, int Y, const wchar_t* text, Color clr)
			{
				if (Settings::Visuals::Visuals::Text.GetInt() == 1)
				{
					int W;
					I::Surface->GetTextSize(Draw::GetESPFont(), text, W, W);
					Draw::Text(text, X - W / 2, Y, Draw::GetESPFont(), clr);
				}
				else
				{
					const wchar_t wchIcon = 0x20 + WEAPON_C4;
					const int iconW = I::Surface->GetCharacterWidth(Draw::fontIcons, wchIcon);

					int textW;
					I::Surface->GetTextSize(Draw::GetESPFont(), text + 2, textW, textW);

					int _X = X - (textW + iconW + 2) / 2;
					int _Y = Y - 1;

					Draw::UnicodeChar(wchIcon, _X, _Y, Draw::fontIcons, clr);
					Draw::Text(text + 2, _X + iconW + 2, _Y, Draw::GetESPFont(), clr);
				}
			}

			bool GetEntityClrItemIndex(C_CSPlayer* player, C_BaseEntity* entity, Color& outClr, short* outItemIndex = nullptr)
			{
				const ClassID classid = entity->GetNetworkable()->GetClientClass()->m_ClassID;
				switch (classid)
				{
				case ClassID::CPlantedC4:
					if (!Settings::Visuals::Visuals::C4.GetInt())
						return false;

					if (!Visuals::GetPlantedC4ClrText(player, static_cast<C_PlantedC4*>(entity), outClr, nullptr, 0))
						return false;

					if (outItemIndex) *outItemIndex = WEAPON_C4;

					return true;
				case ClassID::CC4:
					if (!Settings::Visuals::Visuals::C4.GetInt() || entity->GetOwnerEntity())
						return false;

					outClr.SetRawColor(Settings::Visuals::Visuals::C4Clr.GetInt());
					if (outItemIndex) *outItemIndex = WEAPON_C4;

					return true;
				case ClassID::CEconEntity:
					if (!Settings::Visuals::Visuals::Defuser.GetInt() || (player->GetTeam() != 3) || player->HasDefuser())
						return false;

					outClr.SetRawColor(Settings::Visuals::Visuals::DefuserClr.GetInt());
					if (outItemIndex) *outItemIndex = ITEM_DEFUSER;

					return true;
				case ClassID::CBaseCSGrenadeProjectile:
					if (!Settings::Visuals::Visuals::Grenade::Enable.GetInt())
						return false;

					if (static_cast<C_BaseCSGrenadeProjectile*>(entity)->GetDamage() == 100.f)
					{
						outClr = Color::LightBlue();
						if (outItemIndex) *outItemIndex = WEAPON_FLASHBANG;
					}
					else if (!static_cast<C_BaseCSGrenadeProjectile*>(entity)->GetExplodeEffectTickBegin())
					{
						outClr = Color::LightRed();
						if (outItemIndex) *outItemIndex = WEAPON_HEGRENADE;
					}
					else
						return false;

					return true;
				case ClassID::CDecoyProjectile:
					if (!Settings::Visuals::Visuals::Grenade::Enable.GetInt())
						return false;

					outClr = Color::LightGrey();
					if (outItemIndex) *outItemIndex = WEAPON_DECOY;

					return true;
				case ClassID::CSmokeGrenadeProjectile:
					if (!Settings::Visuals::Visuals::Grenade::Enable.GetInt())
						return false;

					outClr = Color::LightGrey();
					if (outItemIndex) *outItemIndex = WEAPON_SMOKEGRENADE;

					return true;
				case ClassID::CMolotovProjectile:
				case ClassID::CInferno:
					if (!Settings::Visuals::Visuals::Grenade::Enable.GetInt())
						return false;

					outClr = Color::LightRed();
					if (outItemIndex) *outItemIndex = WEAPON_MOLOTOV;

					return true;
				case ClassID::CSensorGrenadeProjectile:
					if (!Settings::Visuals::Visuals::Grenade::Enable.GetInt())
						return false;

					outClr = Color::LightRed();
					if (outItemIndex) *outItemIndex = WEAPON_TAGRENADE;

					return true;
				default:
					if (entity->IsBaseCombatWeapon())
					{
						if (Settings::Visuals::Visuals::Weapon.GetInt() && !entity->GetOwnerEntity())
						{
							short itemIndex = static_cast<C_WeaponCSBaseGun*>(entity)->GetItemDefinitionIndex();

							outClr.SetRawColor(Settings::Visuals::Visuals::WeaponClr.GetInt());
							if (outItemIndex) *outItemIndex = itemIndex;

							return true;
						}
					}
				}
				return false;
			}

			//void DrawInferno(C_Inferno* entity)
			//{
			//	auto BuildCirclePoly = [](const Vector& position, const Vector& xAxis, const Vector& yAxis, float radius, std::vector<vec3>& outPoints)
			//	{
			//		const unsigned int nSegments = 8;
			//		const float flRadStep = (M_PI_F * 2.f) / (float)nSegments;

			//		Vector vecLastPosition;

			//		// Find our first position
			//		// Retained for triangle fanning
			//		Vector vecStart = position + xAxis * radius;
			//		Vector vecPosition = vecStart;

			//		outPoints.push_back(vecStart);

			//		for (unsigned int i = 1; i <= nSegments; ++i)
			//		{
			//			// Store off our last position
			//			vecLastPosition = vecPosition;

			//			// Calculate the new one
			//			float flSin, flCos;
			//			SinCos(flRadStep * i, flSin, flCos);
			//			vecPosition = position + (xAxis * flCos * radius) - (yAxis * flSin * radius);

			//			outPoints.push_back(vecPosition);
			//		}
			//	};

			//	const float radius = 60.f;
			//	const int drawableCount = entity->GetDrawableCount();

			//	std::vector<vec3> points;

			//	for (int i = 0; i < drawableCount; ++i)
			//	{
			//		C_Inferno::Drawable* drawable = entity->GetDrawable(i);

			//		QAngle vecAngles;
			//		VectorAngles(drawable->m_normal, vecAngles);

			//		matrix3x4_t xform;
			//		AngleMatrix(vecAngles, drawable->m_pos, xform);
			//		Vector sideAxis, upAxis;
			//		// default draws circle in the y/z plane
			//		MatrixGetColumn(xform, 2, sideAxis);
			//		MatrixGetColumn(xform, 1, upAxis);

			//		BuildCirclePoly(drawable->m_pos, sideAxis, upAxis, radius, points);
			//	}

			//	//std::vector<size_t> looseEnds;

			//	//bool prevDeleted = false;

			//	//Log::Msg("looseEnds: ");

			//	for (auto it = points.begin(); it != points.end(); )
			//	{
			//		bool deleted = false;

			//		for (int j = 0; j < drawableCount; ++j)
			//		{
			//			C_Inferno::Drawable* drawable = entity->GetDrawable(j);

			//			const float d = (*it - drawable->m_pos).Length2D();
			//			if ((d + 5.f) < radius && sqrtf(Sqr(it->z - drawable->m_pos.z)) < 15.f)
			//			{
			//				it = points.erase(it);
			//				deleted = true;
			//				break;
			//			}
			//		}

			//		/*
			//		if (prevDeleted != deleted)
			//		{
			//			size_t looseEndIndex = (it - points.begin());
			//			if (looseEndIndex != 0 && deleted)
			//				--looseEndIndex;
			//			auto looseEndIt = std::find(looseEnds.begin(), looseEnds.end(), looseEndIndex);
			//			if (looseEndIt == looseEnds.end())
			//			{
			//				Log::Msg("%d, ", looseEndIndex);
			//				looseEnds.push_back(looseEndIndex);
			//			}
			//		}
			//		*/

			//		//prevDeleted = deleted;

			//		if (!deleted)
			//			++it;
			//	}
			//	
			//	/*
			//	Log::Msg("\n");
			//	
			//	for (auto it = points.begin(); (it + 1) != points.end(); ++it)
			//	{
			//		auto end = points.end();

			//		auto found = std::find(looseEnds.begin(), looseEnds.end(), (size_t)(it - points.begin()));

			//		if (found != looseEnds.end())
			//		{
			//			looseEnds.erase(found);

			//			float closestDist = radius;
			//			auto closest = looseEnds.end();

			//			for (auto it2 = looseEnds.begin(); it2 != looseEnds.end(); ++it2)
			//			{
			//				const float d = (*it - points[*it2]).Length2D();
			//				if (d < closestDist && sqrtf(Sqr(it->z - points[*it2].z)) < 15.f)
			//				{
			//					closestDist = d;
			//					closest = it2;
			//				}
			//			}

			//			if (closest == looseEnds.end())
			//				continue;

			//			end = points.begin() + *closest;

			//			looseEnds.erase(closest);
			//		}
			//		else
			//		{
			//			end = (it + 1);

			//			const float d = (*it - *end).Length2D();
			//			if (radius <= d || 15.f <= sqrtf(Sqr(it->z - end->z)))
			//				continue;
			//		}

			//		vec3 screenS, screenE;
			//		if (I::DebugOverlay->ScreenPosition(*it, screenS) &&
			//			I::DebugOverlay->ScreenPosition(*end, screenE))
			//		{
			//			Draw::Line(screenS, screenE, Color(255, 0, 0, 128));
			//			points2.push_back(*it);
			//			points2.push_back(*end);
			//		}
			//	}

			//	for (auto it = points.begin(); (it + 1) != points.end(); ++it)
			//	{
			//		auto end = points.end();

			//		float closestDist = radius;
			//		auto closest = points.end();

			//		for (auto it2 = it + 1; it2 != points.end(); ++it2)
			//		{
			//			if (it == it2)
			//				continue;

			//			const float d = (*it - *it2).Length2D();
			//			if (d < closestDist && sqrtf(Sqr(it->z - it2->z)) < 15.f)
			//			{
			//				closestDist = d;
			//				closest = it2;
			//			}
			//		}

			//		if (closest == points.end())
			//			continue;

			//		end = closest;

			//		vec3 screenS, screenE;
			//		if (I::DebugOverlay->ScreenPosition(*it, screenS) &&
			//			I::DebugOverlay->ScreenPosition(*end, screenE))
			//		{
			//			Draw::Line(screenS, screenE, Color(255, 0, 0, 128));
			//			points2.push_back(*it);
			//			//points2.push_back(*end);
			//		}
			//	}
			//	*/

			//	typedef boost::geometry::model::d2::point_xy<float> point;
			//	typedef boost::geometry::model::polygon<point> polygon;

			//	polygon polyScreen;

			//	for (auto it = points.begin(); it != points.end(); ++it)
			//	{
			//		//vec3 screen;
			//		//if (I::DebugOverlay->ScreenPosition(*it, screen))
			//			boost::geometry::append(polyScreen, point(it->x, it->y));
			//	}

			//	polygon polyConvex;

			//	boost::geometry::convex_hull(polyScreen, polyConvex);
			//	//boost::geometry::correct(polyScreen);

			//	const auto& o = polyConvex.outer();

			//	std::vector<Vertex_t> verts;
			//	
			//	for (auto it = o.begin(); it != o.end(); ++it)
			//	{
			//		vec3 p(boost::geometry::get<0>(*it), boost::geometry::get<1>(*it), points.begin()->z);

			//		vec3 screen;
			//		if (I::DebugOverlay->ScreenPosition(p, screen))
			//		{
			//			verts.push_back(Vertex_t(vec2(screen.x, screen.y)));
			//			Draw::Text(std::to_wstring((size_t)(it - o.begin())).c_str(), screen, Draw::fontESPOutline, Color::White());
			//		}
			//	}

			//	Draw::FilledPoly(verts.data(), verts.size(), Color(255, 0, 0, 32));
			//}

			void DrawGrenadeTimer(int X, int Y, C_BaseEntity* entity, ClassID classid)
			{
				float startTime = 0.f;
				float endTime = 0.f;

				if (classid == ClassID::CDecoyProjectile)
				{
					startTime = static_cast<C_DecoyProjectile*>(entity)->GetParticleInitTime();
					endTime = startTime + (14.f + (0.5f + 4.f) / 2.f);
				}
				else if (classid == ClassID::CSmokeGrenadeProjectile)
				{
					startTime = TICKS_TO_TIME(static_cast<C_SmokeGrenadeProjectile*>(entity)->GetSmokeEffectTickBegin());
					endTime = startTime + (12.5f + 4.55f + 1.f);
				}
				else if (classid == ClassID::CInferno)
				{
					startTime = TICKS_TO_TIME(static_cast<C_Inferno*>(entity)->GetFireEffectTickBegin());
					static ConVar* inferno_flame_lifetime = I::Cvar->FindVar("inferno_flame_lifetime");
					endTime = startTime + inferno_flame_lifetime->GetFloat();
				}

				if (startTime)
				{
					DrawHorizontalProgressBar(X - 20, X + 20,
						Y + (Settings::Visuals::Visuals::Box.GetInt() ? 2 : 0),
						G::Globals->curtime, endTime, startTime, Color::Black());
				}
			}

			void Paint(C_CSPlayer* player, C_BaseEntity* entity)
			{
				short itemIndex;
				Color clr;

				if (!GetEntityClrItemIndex(player, entity, clr, &itemIndex))
					return;

				int left, right,
					top, bottom;

				if (Settings::Visuals::Visuals::Box.GetInt())
				{
					if (!BoundingBoxToScreen(entity, left, top, right, bottom))
						return;

					if (left != right || top != bottom)
						DrawBox(left, top, right, bottom, clr, false);
				} // remove this check in the future
				else if (Settings::Visuals::Visuals::C4.GetInt() ||
					Settings::Visuals::Visuals::Text.GetInt() ||
					Settings::Visuals::Visuals::Grenade::Enable.GetInt())
				{
					vec3 screen;
					if (!I::DebugOverlay->ScreenPosition(entity->GetAbsOrigin(), screen))
						return;

					left = right = screen.x;
					top = bottom = screen.y;
				}
				else
					return;

				const int center = right - (right - left) / 2;
				// a pixel outside the box
				int nextOverheadY = top - 1 - Settings::Visuals::ESP::Style.GetInt();

				const ClassID classid = entity->GetNetworkable()->GetClientClass()->m_ClassID;
				if (Settings::Visuals::Visuals::C4.GetInt() && classid == ClassID::CPlantedC4)
				{
					wchar_t text[24];
					if (GetPlantedC4ClrText(player, static_cast<C_PlantedC4*>(entity), clr, text, _countof(text)))
						DrawPlantedC4(center, nextOverheadY - 13, text, clr);

					return;
				}

				if (Settings::Visuals::Visuals::Text.GetInt())
				{
					DrawWeapon(center, nextOverheadY,
						true,
						(Settings::Visuals::Visuals::Text.GetInt() == 2),
						Settings::Visuals::Visuals::WeaponAmmo.GetInt(),
						entity, itemIndex, clr);
				}

				if (Settings::Visuals::Visuals::Grenade::Enable.GetInt() &&
					(classid == ClassID::CBaseCSGrenadeProjectile ||
					classid == ClassID::CDecoyProjectile ||
					classid == ClassID::CSmokeGrenadeProjectile ||
					classid == ClassID::CMolotovProjectile ||
					classid == ClassID::CInferno ||
					classid == ClassID::CSensorGrenadeProjectile))
				{
					//if (Settings::Visuals::Visuals::Grenade::Range.GetInt() && classid == ClassID::CInferno)
					//	DrawInferno(static_cast<C_Inferno*>(entity));

					if (Settings::Visuals::Visuals::Grenade::Thrower.GetInt())
					{
						C_BaseEntity* owner = entity->GetOwnerEntity();
						if (owner)
						{
							const int index = owner->entindex();
							DrawName(center, nextOverheadY, index,
								((*G::pGameRules)->IsPlayingAnyCompetitiveStrictRuleset() ? (*G::pPR)->GetCompTeammateColor(index) : clr),
								true, (owner->GetTeam() == player->GetTeam()));
						}
					}

					if (Settings::Visuals::Visuals::Grenade::Timer.GetInt())
						DrawGrenadeTimer(center, bottom, entity, classid);
				}
			}
		}

		void Glow()
		{
			C_CSPlayer* player = static_cast<C_CSPlayer*>(I::EntityList->GetClientEntity(I::EngineClient->GetLocalPlayer()));
			if (!player)
				return;

			for (int i = G::GlowObjectManager->m_GlowObjectDefinitions.Count() - 1; i >= 0; --i)
			{
				GlowObjectDefinition_t& obj = G::GlowObjectManager->m_GlowObjectDefinitions[i];

				if (obj.IsUnused() || !obj.m_pEntity || (obj.m_pEntity == player) || obj.m_pEntity->IsDormant())
					continue;

				Color clr;

				if (obj.m_pEntity->entindex() <= G::Globals->maxClients)
				{
					if (!Settings::Visuals::Glow::Enable.GetInt())
						continue;

					bool sameTeam = (obj.m_pEntity->GetTeam() == player->GetTeam());
					if (!Settings::Visuals::Glow::Teammates.GetInt() && sameTeam)
						continue;

					if (Settings::Visuals::Glow::VisCheck.GetInt() &&
						(player->IsBlinded() || !CanSeeEachOther(player->GetEyePos(), obj.m_pEntity->WorldSpaceCenter(), player, obj.m_pEntity, true)))
						continue;

					clr.SetRawColor((sameTeam ? Settings::Visuals::Glow::clrTeam : Settings::Visuals::Glow::clrEnemy).GetInt());

					obj.m_nRenderStyle = Settings::Visuals::Glow::Style.GetInt();
				}
				else
				{
					if (!Settings::Visuals::Visuals::Enable.GetInt() || !Settings::Visuals::Visuals::Glow.GetInt() || 
						!Visuals::GetEntityClrItemIndex(player, obj.m_pEntity, clr, nullptr))
						continue;

					clr.r *= 0.7f;
					clr.g *= 0.7f;
					clr.b *= 0.7f;
				}

				obj.m_GlowColor[0] = clr.r / 255.f;
				obj.m_GlowColor[1] = clr.g / 255.f;
				obj.m_GlowColor[2] = clr.b / 255.f;
				obj.m_GlowColor[3] = clr.a / 255.f;

				obj.m_bRenderWhenOccluded = true;

				// glow becomes bolder when shooting, disable that
				//obj.m_flGlowPulseOverdrive = 0.f;
			}
		}

		void DrawBacktrack(C_CSPlayer* entity, int entIndex)
		{
			if (!entity->IsAlive())
				return;

			Moves::Aim::Backtrack::CRecord* oldest = nullptr;

			for (int i = 0; i < Moves::Aim::Backtrack::ticksStored; ++i)
			{
				// -1 cos we skip the world
				Moves::Aim::Backtrack::CRecord* rec = &Moves::Aim::Backtrack::records[entIndex - 1][i];

				if (!Moves::Aim::Backtrack::IsWithinUnlag(rec->time))
					continue;

				if (!oldest || (rec->time < oldest->time))
					oldest = rec;
			}

			if (oldest)
			{
				matrix3x4_t* pmatrix[MAXSTUDIOBONES];
				for (int i = 0; i < MAXSTUDIOBONES; ++i)
					pmatrix[i] = &oldest->matrix[i];

				ESP::DrawSkeleton(entity, pmatrix, Color(Settings::Moves::Aim::Backtrack::DrawClr.GetInt()));
			}
		}

		void Chams(Color clrVis, Color clrNotVis, bool visCheck, int style, bool wireframe,
			DrawModelExecuteFn DrawModelExecute,
			IVModelRender* render, void* pRenderContext, const DrawModelState_t& pState, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
		{
			static IMaterial* matLitNotVis = Features::Visuals::CreateChamsMaterial(true, true);
			static IMaterial* matLitVis = Features::Visuals::CreateChamsMaterial(true, false);
			static IMaterial* matUnlitNotVis = Features::Visuals::CreateChamsMaterial(false, true);
			static IMaterial* matUnlitVis = Features::Visuals::CreateChamsMaterial(false, false);

			IMaterial* matNotVis = nullptr;
			IMaterial* matVis = nullptr;

			if (style)
			{
				matNotVis = matUnlitNotVis;
				matVis = matUnlitVis;
			}
			else
			{
				matNotVis = matLitNotVis;
				matVis = matLitVis;
			}

			if (!visCheck)
			{
				matNotVis->ColorModulate(clrNotVis.r / 255.f, clrNotVis.g / 255.f, clrNotVis.b / 255.f);
				matNotVis->AlphaModulate(clrNotVis.a / 255.f);
				matNotVis->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, wireframe);

				render->ForcedMaterialOverride(matNotVis);
				DrawModelExecute(render, pRenderContext, pState, pInfo, pCustomBoneToWorld);
			}

			matVis->ColorModulate(clrVis.r / 255.f, clrVis.g / 255.f, clrVis.b / 255.f);
			matVis->AlphaModulate(clrVis.a / 255.f);
			matVis->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, wireframe);

			render->ForcedMaterialOverride(matVis);
			DrawModelExecute(render, pRenderContext, pState, pInfo, pCustomBoneToWorld);
			I::ModelRender->ForcedMaterialOverride(nullptr);
		}

		void DrawAntiAim(C_CSPlayer* player, int screenW, int screenH)
		{
			if (!I::Input->CAM_IsThirdPerson())
			{
				const int dirPosW = screenW / 2;
				const int dirPosH = screenH / 2 + 200;

				Draw::Arrow(dirPosW, dirPosH, Moves::AntiAim::realSideRight ? -90.f : 90.f, Color::White());

				return;
			}

			vec3 start(player->GetAbsOrigin());
			vec3 startScreen;
			if (!I::DebugOverlay->ScreenPosition(start, startScreen))
				return;

			const float lineLen = 32.f;

			vec3 lby(0.f, player->GetLowerBodyYawTarget(), 0.f);
			vec3 lbyFwd;
			AngleVectors(lby, &lbyFwd);

			vec3 lbyEnd(start + (lbyFwd * lineLen));
			vec3 lbyEndScreen;
			if (I::DebugOverlay->ScreenPosition(lbyEnd, lbyEndScreen))
			{
				Draw::Line(startScreen, lbyEndScreen, Color::Yellow());
				Draw::Text(L"L", lbyEndScreen, Draw::GetESPFont(), Color::White());
			}

			vec3 real(0.f, Moves::AntiAim::lastRealYaw, 0.f);
			vec3 realFwd;
			AngleVectors(real, &realFwd);

			vec3 realEnd(start + (realFwd * lineLen));
			vec3 realEndScreen;
			if (I::DebugOverlay->ScreenPosition(realEnd, realEndScreen))
			{
				Draw::Line(startScreen, realEndScreen, Color::Blue());
				Draw::Text(L"R", realEndScreen, Draw::GetESPFont(), Color::White());
			}
		}

		void DrawSpectators(C_CSPlayer* player, int indicatorX, int& indicatorY)
		{
			Draw::Text(L"spectators:", indicatorX, indicatorY, Draw::fontUI, Color::White());

			for (int i = 1; i <= G::Globals->maxClients; ++i)
			{
				C_CSPlayer* entity = static_cast<C_CSPlayer*>(I::EntityList->GetClientEntity(i));
				if (!entity || (entity == player) || entity->IsDormant() || entity->IsAlive())
					continue;

				if (entity->GetObserverTarget() != player)
					continue;

				player_info_t info;
				if (!I::EngineClient->GetPlayerInfo(i, &info) || info.fakeplayer)
					continue;

				wchar_t print[40] = { 0 };
				if (!MultiByteToWideChar(CP_UTF8, 0, info.name, _countof(print) - 1, print, _countof(print)))
					continue;

				indicatorY += lineSpacing;

				Draw::Text(print, indicatorX, indicatorY, Draw::fontUI, Color::White());
			}

			//indicatorY += lineHeight;
		}

		void DrawScoreboard(C_CSPlayer* localPlayer, int screenW, int screenH)
		{
			if (!Moves::lastCmd || !(Moves::lastCmd->buttons & IN_SCORE))
				return;

			C_CSPlayerResource* pr = (*G::pPR);

			if (!(*G::pGameRules) || !(*G::pGameRules)->IsQueuedMatchmaking() || !pr)
				return;

			if (!Settings::Misc::Log::Wins.GetInt() && !Settings::Misc::Log::TeamDmg.GetInt())
				return;

			enum Stats
			{
				SCORE,
				MVPS,
				KILLS,
				ASSISTS,
				DEATHS,
				LEADER,
				TEACHER,
				FRIENDLY,
				RANK,
				DAMAGE,

				LAST
			};

			struct StatSort
			{
				Stats stat;
				bool lessBetter;
			};

			// https://github.com/sapphyrus/panorama/blob/master/scripts/scoreboard.js#L679
			const StatSort sortOrderDefault[] = {
				{ SCORE, false },
				{ MVPS, false },
				{ KILLS, false },
				{ ASSISTS, false },
				{ DEATHS, true },
				{ LEADER, false },
				{ TEACHER, false },
				{ FRIENDLY, false },
				{ RANK, false },

				{ LAST, false }
			};

			const StatSort sortOrderPremier[] = {
				{ DAMAGE, false },
				{ KILLS, false },
				{ MVPS, false },
				{ ASSISTS, false },
				{ DEATHS, true },
				{ LEADER, false },
				{ TEACHER, false },
				{ FRIENDLY, false },
				{ RANK, false },

				{ LAST, false }
			};

			const StatSort* sortOrders[] = {
				sortOrderDefault,
				sortOrderPremier
			};

			int sortOrderIndex = 0;

			if ((*G::pGameRules)->GetQueuedMatchmakingMode())
				sortOrderIndex = 1;

			class PlayerStats
			{
			public:
				int index;
				int stats[LAST];

				void Set(C_CSPlayerResource* pr, int entIndex, bool isTeammate)
				{
					index = entIndex;
					stats[SCORE] = pr->GetScore(index);
					stats[MVPS] = pr->GetNumMVPs(index);
					stats[KILLS] = pr->GetKills(index);
					stats[ASSISTS] = pr->GetAssists(index);
					stats[DEATHS] = pr->GetDeaths(index);
					// csgo only sorts local team by commends
					// https://github.com/sapphyrus/panorama/blob/master/scripts/scoreboard.js#L2043
					if (isTeammate)
					{
						stats[LEADER] = pr->GetPersonaDataPublicCommendsLeader(index);
						stats[TEACHER] = pr->GetPersonaDataPublicCommendsTeacher(index);
						stats[FRIENDLY] = pr->GetPersonaDataPublicCommendsFriendly(index);
					}
					else
					{
						stats[LEADER] = 0;
						stats[TEACHER] = 0;
						stats[FRIENDLY] = 0;
					}
					stats[RANK] = pr->GetPersonaDataPublicLevel(index);
					stats[DAMAGE] = pr->GetDamageTotal(index);
				}
			};

			PlayerStats playersT[5], playersCT[5];
			int countT = 0, countCT = 0;

			const int localTeam = localPlayer->GetTeam();

			for (int i = 1; i <= G::Globals->maxClients; ++i)
			{
				if (!pr->IsConnected(i))
					continue;

				const int team = pr->GetTeam(i);
				if (team == 2)
				{
					if (countT < _countof(playersT))
					{
						playersT[countT].Set(pr, i, (localTeam == team));
						++countT;
					}
				}
				else if (team == 3)
				{
					if (countCT < _countof(playersCT))
					{
						playersCT[countCT].Set(pr, i, (localTeam == team));
						++countCT;
					}
				}
			}

			PlayerStats* sortedT[_countof(playersT)];
			for (int i = 0; i < countT; ++i)
				sortedT[i] = &playersT[i];

			PlayerStats* sortedCT[_countof(playersCT)];
			for (int i = 0; i < countCT; ++i)
				sortedCT[i] = &playersCT[i];

			auto Sort = [&](PlayerStats** arr, int len)
			{
				for (int i = 0; i < len; ++i)
				{
					for (int j = i + 1; j < len; ++j)
					{
						for (int k = 0; 
							sortOrders[sortOrderIndex][k].stat != LAST;
							++k)
						{
							const StatSort& statSort = sortOrders[sortOrderIndex][k];
							if (statSort.lessBetter)
							{
								if (arr[i]->stats[statSort.stat] > arr[j]->stats[statSort.stat])
								{
									PlayerStats* tmp = arr[i];
									arr[i] = arr[j];
									arr[j] = tmp;
									break;
								}
								else if (arr[i]->stats[statSort.stat] < arr[j]->stats[statSort.stat])
									break;
							}
							else
							{
								if (arr[i]->stats[statSort.stat] < arr[j]->stats[statSort.stat])
								{
									PlayerStats* tmp = arr[i];
									arr[i] = arr[j];
									arr[j] = tmp;
									break;
								}
								else if (arr[i]->stats[statSort.stat] > arr[j]->stats[statSort.stat])
									break;
							}
						}
					}
				}
			};

			Sort(sortedT, countT);
			Sort(sortedCT, countCT);

			const int xFirst = screenW * 0.755f;
			int x = xFirst;
			constexpr int widthWins = 49;
			constexpr int widthTeamDmg = 86;

			int y = screenH * 0.325f;
			const int spacing = screenH * 0.025f;

			if (Settings::Misc::Log::Wins.GetInt())
			{
				Draw::Text(L"Wins", x, y, Draw::fontUI, Color::White());
				x += widthWins;
			}

			if (Settings::Misc::Log::TeamDmg.GetInt())
			{
				Draw::Text(L"Team-dmg", x, y, Draw::fontUI, Color::White());
				x += widthTeamDmg;
				Draw::Text(L"Team-kills", x, y, Draw::fontUI, Color::White());
			}

			auto Draw = [&](PlayerStats** arr, int len, bool isTeammate)
			{
				for (int i = 0; i < len; ++i)
				{
					y += spacing;

					const int index = arr[i]->index;

					// skip drawing info for bots
					if (pr->GetBotDifficulty(index) != -1)
						continue;

					x = xFirst;

					wchar_t print[28] = { 0 };

					if (Settings::Misc::Log::Wins.GetInt())
					{
						const int winCount = pr->GetCompetitiveWins(index);
						swprintf_s(print, _countof(print), L"%d", winCount);
						Draw::Text(print, x, y, Draw::fontUI, Color::White());
						
						x += widthWins;
					}

					if (isTeammate && Settings::Misc::Log::TeamDmg.GetInt())
					{
						const int dmg = Misc::Log::teamDmg[index - 1].dmg;
						if (dmg)
						{
							swprintf_s(print, _countof(print), L"%d", dmg);
							Draw::Text(print, x, y, Draw::fontUI, Color::White());
						}
						x += widthTeamDmg;

						const int kills = Misc::Log::teamDmg[index - 1].kills;
						if (kills)
						{
							swprintf_s(print, _countof(print), L"%d", kills);
							Draw::Text(print, x, y, Draw::fontUI, Color::White());
						}
					}
				}
			};

			Draw(sortedCT, countCT, (localTeam == 3));

			y = screenH * 0.543f;

			Draw(sortedT, countT, (localTeam == 2));
		}

		void DrawIndicators(int indicatorX, int& indicatorY)
		{
			if (Settings::Misc::FakeLag::Enable.GetInt() && Settings::Misc::FakeLag::ToggleKey.GetInt() && Misc::fakeLagOn)
			{
				Draw::Text(L"fakelagging", indicatorX, indicatorY, Draw::fontUI, Color::White());
				indicatorY += lineSpacing;
			}

			if (Settings::Moves::AutoBlockKey.GetInt() && (Moves::autoBlockTarget != -1))
			{
				Draw::Text(L"autoblocking", indicatorX, indicatorY, Draw::fontUI, Color::White());
				indicatorY += lineSpacing;
			}
		}

		void DrawSniperCrossLines(C_CSPlayer* localPlayer, int screenW, int screenH)
		{
			C_WeaponCSBaseGun* weapon = localPlayer->GetActiveWeapon();
			if (!weapon)
				return;

			short index = weapon->GetItemDefinitionIndex();

			if (Settings::Visuals::NoSniperScope.GetInt() && localPlayer->IsScoped() &&
				(index != WEAPON_AUG) && (index != WEAPON_SG556))
			{
				static ConVar* cl_crosshair_sniper_width = I::Cvar->FindVar("cl_crosshair_sniper_width");
				int lineWidth = cl_crosshair_sniper_width->GetInt();
				if (lineWidth < 1)
					lineWidth = 1;

				const int halfLineWidth = lineWidth / 2;

				const int x = screenW / 2;
				const int y = screenH / 2;

				Draw::FilledRect(0, y - halfLineWidth, screenW, y + lineWidth - halfLineWidth, Color::Black());
				Draw::FilledRect(x - halfLineWidth, 0, x + lineWidth - halfLineWidth, screenH, Color::Black());
			}
		}

		void Paint()
		{
			C_CSPlayer* localPlayer = static_cast<C_CSPlayer*>(I::EntityList->GetClientEntity(I::EngineClient->GetLocalPlayer()));
			if (!localPlayer)
				return;

			for (int i = I::EntityList->GetHighestEntityIndex(); i > 0; --i)
			{
				C_BaseEntity* entity = I::EntityList->GetClientEntity(i);
				if (!entity || (entity == localPlayer) || entity->IsDormant())
					continue;

				if (i <= G::Globals->maxClients)
				{
					C_CSPlayer* entityPlayer = static_cast<C_CSPlayer*>(entity);

					if (Settings::Visuals::ESP::Enable.GetInt())
						ESP::Draw(localPlayer, entityPlayer, i);

					if (Settings::Moves::Aim::Backtrack::Enable.GetInt() &&
						Settings::Moves::Aim::Backtrack::Draw.GetInt())
						DrawBacktrack(entityPlayer, i);

					if (Settings::Visuals::Radar.GetInt())
						entity->SetSpotted(true);
				}
				else if (Settings::Visuals::Visuals::Enable.GetInt())
					Visuals::Paint(localPlayer, entity);
			}

			int screenW, screenH;
			I::EngineClient->GetScreenSize(screenW, screenH);

			DrawScoreboard(localPlayer, screenW, screenH);

			if (!localPlayer->IsAlive())
			{
				C_CSPlayer* obsTarget = static_cast<C_CSPlayer*>(localPlayer->GetObserverTarget());
				if (obsTarget)
					DrawSniperCrossLines(obsTarget, screenW, screenH);
				return;
			}

			DrawSniperCrossLines(localPlayer, screenW, screenH);

			if (Settings::Moves::AntiAim::Enable.GetInt() && Settings::Moves::AntiAim::Desync.GetInt())
				DrawAntiAim(localPlayer, screenW, screenH);

			constexpr int indicatorX = 18;
			int indicatorY = screenH / 2.3f;

			DrawIndicators(indicatorX, indicatorY);

			if (Settings::Visuals::Spectators.GetInt())
				DrawSpectators(localPlayer, indicatorX, indicatorY);
		}

		CProxyRecvHook hkDidSmokeEffect;

		void RecvProxy_DidSmokeEffect(const CRecvProxyData* pData, void* pStruct, void* pOut)
		{
			static int prev = 0;
			int cur = Settings::Visuals::NoSmoke.GetInt();
			if (prev != cur)
			{
				const char* materials[] = {
					"particle/vistasmokev1/vistasmokev1_emods",
					"particle/vistasmokev1/vistasmokev1_emods_impactdust",
					"particle/vistasmokev1/vistasmokev1_fire",
					"particle/vistasmokev1/vistasmokev1_smokegrenade",
				};

				for (size_t i = 0; i < _countof(materials); ++i)
				{
					IMaterial* m = I::MaterialSystem->FindMaterial(materials[i], TEXTURE_GROUP_OTHER);
					m->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, (cur == 2));
				}

				prev = cur;
			}

			if (cur == 1) // m_bSmokeEffectSpawned
				*reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(pOut) + 0x8) = true;

			hkDidSmokeEffect.original(pData, pStruct, pOut);
		}
	}
}