Out of process plasmoids

Rollout plan:
  - make bodge system for hosting random processes

  - make infra for hosting random custom content OOP in a proper organised way (see below)

  - make a process that mostly shoves existing content into this interface. Doesn't need to be API compatiable

  - start port clients away from using Plasma::Applet

  - port some of our obscure applets (like weather && notes or something)

  - port our main applets

 DBus host has
 /Plasma/Applets <--- Has FactoryIface
 /Plasma/AppletsID_HERE/2343  <---- PlasmoidIface
 /Plasma/AppletsID_HERE/123 <---- PlasmoidIface

FactoryIface:
  QDBusObjectPath  createApplet(string applet, int id, variantmap of random properties)

PlasmoidIface:
 - set/getConfig
 - busy/title/whatever
 WaylandResourceId   visualWindow
 WaylandResourceId   configWindow


----

Binary that hosts plasma applets from a hardcoded list
Binary that hosts generic locally installed applet

Could be the same thing, just we make a small script run on startup
plasmoidHost org.kde.plasma.battery org.kde.plasma.taskmanager ......

Inside our host, we should make it create the factory right away, but lazy load creating the actual applets.


Maybe inherit from XDG::TopLevel (but not XDGShellV6...would need Qt changes to do this)
-----

Design Goals:

* Someone should be able to write an applet using absolutely zero KDE libs: Just DBus and wayland.

* Should be able to host an applet from an existing process
i.e battery applet inside powerdevil

Not necessarily because we actually want these things, but because it forces things to be layered things properly.

Avoid complexities of compact/full at a protocol level.
We have a main view, that can create popups as windows. The applet host can do whatever it wants to make that happen

----

Open Questions:
Who holds the config?
Scripting?
Config UI
