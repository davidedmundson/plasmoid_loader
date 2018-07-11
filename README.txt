Out of process plasmoids

Rollout plan:
  - make bodge system for hosting random processes (DONE)

  - make infra for hosting random custom content OOP in a proper organised way (WIP)

  - make a process that mostly shoves existing content into this interface. (WIP)

  - port some of our obscure kdeplasma-addons applet to use it (like binary-clock or something)

  - deploy

  - port our main applets

  ====

  Folder contains two examples:

- embed window plasmoid

A user-configurable plasmoid for running an arbitrary app wtih args and stuff and having it in a plasmoid

 - plasmoid loader

 A fully fledged design of loading an applet out of process and embedding it

  =====

Embedding applets

 DBus host has
 /Plasma/Applets/Factory/AnalogClock <--- Has FactoryIface

 /Plasma/Applets/Instance/343  <---- PlasmoidIface
 /Plasma/Applets/Instance/123 <---- PlasmoidIface

 DBus host service + path for factory will be written in relevant .desktop file

 (This still means plasmashell will have to parse packages....boo!!!!!. Is there a sane way to avoid this that still allows DBus activation?)

FactoryIface:
  QDBusObjectPath  createApplet(string applet, int id, variantmap of random properties)

PlasmoidIface:
 - set/getConfig
 - busy/title/whatever
 WaylandResourceId   visualWindow
 WaylandResourceId   configWindow



 WaylandResourceId  will be some reference to the surface in clever magic way I've not decided upon



----

We need a

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
