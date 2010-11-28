/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2010 The XCSoar Project
  A detailed list of copyright holders can be found in the file "AUTHORS".

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

#include "GlueMapWindow.hpp"

#include "Screen/Graphics.hpp"
#include "Screen/Icon.hpp"
#include "Components.hpp"
#include "Interface.hpp"
#include "Task/ProtectedTaskManager.hpp"
#include "Screen/Layout.hpp"

void
GlueMapWindow::TargetPaintDrag(Canvas &canvas, const RasterPoint drag_last)
{
  Graphics::hBmpTarget.draw(canvas, drag_last.x, drag_last.y);
}

bool
GlueMapWindow::TargetDragged(const int x, const int y)
{
  GeoPoint gp = visible_projection.ScreenToGeo(x, y);
  if (protected_task_manager.target_is_locked(
                             XCSoarInterface::SettingsMap().TargetPanIndex)) {
    protected_task_manager.set_target(
                           XCSoarInterface::SettingsMap().TargetPanIndex, gp, true);
    return true;
  }
  return false;
}

bool
GlueMapWindow::isClickOnTarget(const RasterPoint pc)
{
  if (XCSoarInterface::SettingsMap().TargetPan) {
    if (!protected_task_manager.target_is_locked(
                                XCSoarInterface::SettingsMap().TargetPanIndex))
      return false;

    GeoPoint gnull;
    const GeoPoint& t = protected_task_manager.get_location_target(
        XCSoarInterface::SettingsMap().TargetPanIndex, gnull);

    if (t == gnull)
      return false;

    const GeoPoint gp = visible_projection.ScreenToGeo(pc.x, pc.y);
    if (visible_projection.GeoToScreenDistance(gp.distance(t)) <
        unsigned(Layout::Scale(10)))
      return true;
  }
  return false;
}

bool
GlueMapWindow::isInSector(const int x, const int y)
{
  if (XCSoarInterface::SettingsMap().TargetPan) {
    GeoPoint gp = visible_projection.ScreenToGeo(x, y);
    AIRCRAFT_STATE a;
    a.Location = gp;
    return protected_task_manager.isInSector(
                                  XCSoarInterface::SettingsMap().TargetPanIndex, a);
  }
  return false;
}
