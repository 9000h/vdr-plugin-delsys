/*
 * Delivery System plugin for the Video Disk Recorder
 *
 * delsys.c: main source file
 *
 * Copyright (C) 2011 Oliver Endriss <o.endriss@gmx.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * Or, point your browser to http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 */

#include <vdr/plugin.h>

static const char *VERSION        = "0.0.1";
static const char *DESCRIPTION    = "Allows to disable delivery systems for a device";
static const char *MAINMENUENTRY  = "Delivery System";


//
// class cDeliverySystemDeviceHook
//
class cDeliverySystemDeviceHook : public cDeviceHook {
private:
  enum delsysAction { action_default, action_deny };
  delsysAction dvb_c_action[MAXDEVICES];
  delsysAction dvb_t_action[MAXDEVICES];
  delsysAction dvb_s_action[MAXDEVICES];
public:
  cDeliverySystemDeviceHook(void);
  virtual bool DeviceProvidesTransponder(const cDevice *Device, const cChannel *Channel) const;
};

cDeliverySystemDeviceHook::cDeliverySystemDeviceHook()
{
  for (int i = 0; i < MAXDEVICES; i++) {
      dvb_c_action[i] = action_default;
      dvb_t_action[i] = action_default;
      dvb_s_action[i] = action_default;
  }

  // TODO: configuration
  dvb_c_action[0] = action_deny; // device 0: no DVB-C 
  dvb_t_action[1] = action_deny; // device 1: no DVB-T
}

bool cDeliverySystemDeviceHook::DeviceProvidesTransponder(const cDevice *Device, const cChannel *Channel) const
{
  int n = Device->DeviceNumber();

  //printf("%s: device %u\n", __func__, n);

  if (Channel->IsTerr()) {
      if (dvb_t_action[n] == action_deny)
          return false;
      else
          return true;
  }

  if (Channel->IsCable()) {
      if (dvb_c_action[n] == action_deny)
          return false;
      else
          return true;
  }

  if (Channel->IsSat()) {
       if (dvb_s_action[n] == action_deny)
          return false;
      else
          return true;
  }

  // any other delivery system
  return true;
}


class cPluginDelsys : public cPlugin {
private:
  // Add any member variables or functions you may need here.
public:
  cPluginDelsys(void);
  virtual ~cPluginDelsys();
  virtual const char *Version(void) { return VERSION; }
  virtual const char *Description(void) { return DESCRIPTION; }
  virtual const char *CommandLineHelp(void);
  virtual bool ProcessArgs(int argc, char *argv[]);
  virtual bool Initialize(void);
  virtual bool Start(void);
  virtual void Stop(void);
  virtual void Housekeeping(void);
  virtual void MainThreadHook(void);
  virtual cString Active(void);
  virtual time_t WakeupTime(void);
  virtual const char *MainMenuEntry(void) { return MAINMENUENTRY; }
  virtual cOsdObject *MainMenuAction(void);
  virtual cMenuSetupPage *SetupMenu(void);
  virtual bool SetupParse(const char *Name, const char *Value);
  virtual bool Service(const char *Id, void *Data = NULL);
  virtual const char **SVDRPHelpPages(void);
  virtual cString SVDRPCommand(const char *Command, const char *Option, int &ReplyCode);
  };

cPluginDelsys::cPluginDelsys(void)
{
  // Initialize any member variables here.
  // DON'T DO ANYTHING ELSE THAT MAY HAVE SIDE EFFECTS, REQUIRE GLOBAL
  // VDR OBJECTS TO EXIST OR PRODUCE ANY OUTPUT!
}

cPluginDelsys::~cPluginDelsys()
{
  // Clean up after yourself!
}

const char *cPluginDelsys::CommandLineHelp(void)
{
  // Return a string that describes all known command line options.
  return NULL;
}

bool cPluginDelsys::ProcessArgs(int argc, char *argv[])
{
  // Implement command line argument processing here if applicable.
  return true;
}

bool cPluginDelsys::Initialize(void)
{
  // Initialize any background activities the plugin shall perform.
  new cDeliverySystemDeviceHook;
  return true;
}

bool cPluginDelsys::Start(void)
{
  // Start any background activities the plugin shall perform.
  return true;
}

void cPluginDelsys::Stop(void)
{
  // Stop any background activities the plugin is performing.
}

void cPluginDelsys::Housekeeping(void)
{
  // Perform any cleanup or other regular tasks.
}

void cPluginDelsys::MainThreadHook(void)
{
  // Perform actions in the context of the main program thread.
  // WARNING: Use with great care - see PLUGINS.html!
}

cString cPluginDelsys::Active(void)
{
  // Return a message string if shutdown should be postponed
  return NULL;
}

time_t cPluginDelsys::WakeupTime(void)
{
  // Return custom wakeup time for shutdown script
  return 0;
}

cOsdObject *cPluginDelsys::MainMenuAction(void)
{
  // Perform the action when selected from the main VDR menu.
  return NULL;
}

cMenuSetupPage *cPluginDelsys::SetupMenu(void)
{
  // Return a setup menu in case the plugin supports one.
  return NULL;
}

bool cPluginDelsys::SetupParse(const char *Name, const char *Value)
{
  // Parse your own setup parameters and store their values.
  return false;
}

bool cPluginDelsys::Service(const char *Id, void *Data)
{
  // Handle custom service requests from other plugins
  return false;
}

const char **cPluginDelsys::SVDRPHelpPages(void)
{
  // Return help text for SVDRP commands this plugin implements
  return NULL;
}

cString cPluginDelsys::SVDRPCommand(const char *Command, const char *Option, int &ReplyCode)
{
  // Process SVDRP commands this plugin implements
  return NULL;
}

VDRPLUGINCREATOR(cPluginDelsys); // Don't touch this!
