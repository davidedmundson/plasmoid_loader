Out of process plasmoids

Rollout plan:
  - make bodge system for hosting random processes as a demo (DONE) - 2017q4

  - fix popups, use direct EGL (DONE) - 2018q2

  - make infra for hosting random custom content OOP in a proper organised way (WIP)

  - make a process that mostly shoves existing content into this interface.
  (want to do it in this order, so we base design around what we want, not designing API for shimming existig stuff)

  - port some of our obscure kdeplasma-addons applet to use it (like binary-clock or something)

  - deploy that

  - fix up so that all our main plasmoids work (except maybe notifications)

  - make plasmashell do this by default for store plasmoids (probably 2 years from now)

  - replace plasmashell binary with a thinner client wrapper that only wraps external stuff? Plasma 6.0 material (easily 3-4 years from now)

  ====

  Folder contains two examples:

- embed window plasmoid

A user-configurable plasmoid for running an arbitrary app wtih args and stuff and having it in a plasmoid

 - plasmoid loader

 A fully fledged design of a daemon () that hosts a plasmoid and a wrapper plasmoid that shows it.

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
 - visualWindow returning an ID to the wayland window
 - configWindow(s)

(dream plan: plasma scripting for all things also becomes out of process! Our scripts wrap relevant DBus calls)

wayland window ID  will be some reference to the surface in clever magic way I've not decided upon (maybe xdg-foreign?)
or maybe we just attach a name/id to the surface shell and match things up that way.

Wayland surface to attach plasmoids (dream: replace xdg_toplevel  )

two toplevels:
    plasma_applet
    plasma_config


Config is interesting, as have the embedding KCMs in plasmoid conifgs, so we need to generate the host window in the shell (like it is now)
remotely transfer a list of names + icons of config modules, and then embed those pages indivdually.


-----

Design Goals:

* Someone should be able to write an applet using absolutely zero KDE libs: Just DBus and wayland.

* Should be able to host an applet from an existing process
i.e battery applet inside powerdevil

Not necessarily because we actually want these things, but because it forces things to be layered things properly.

Avoid complexities of compact/full at a protocol level.
We have a main view, that can create popups as windows. The applet host can do whatever it wants to make that happen

KCMs out of process in systemsettings might become a thing, we need to keep abstract stuff abstract

----

Open Questions:
Who holds the config?
Scripting?
Config UI

----

FAQ:

Does it work on X?
Yes.

Something something nvidia
It's fine

Why not an IVI shell approach?
Won't work on X

What's most problematic?
bloody tooltips!
