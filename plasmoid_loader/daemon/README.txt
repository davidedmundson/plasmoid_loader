This daemon will host the digital clock (as a chosen specific example) in a daemon

It's sort of a clone of plasmawindowed but with a custom applet for form factor and stuff


DBus API to create and manage Applets + expose config to scripting
(dream plan: plasma scripting is also out of process! Our wrappers wrap DBus calls)


Wayland surface created with an ID passed to match things up
(get surface ID from DBus (xdg-foreign?) or just create surface regardless with an ID that matches up?)

What goes into daemon, what goes into the wayland API?
 - title
 - trigger to create config surfaces
            - we have multiple config pages with KCM embedding, so pagestack should not be embedded, but content invidudually?



Wayland surface to attach plasmoids (dream: replace xdg_toplevel  )

two toplevels:
    plasma_applet
    plasma_config

(maybe applet loads the config surfaces)

<request name="set_applet">
    Set name of the applet so shell can attach in the right place
    <arg name="applet" type="string">
    <arg name="id" type="int">
</request>

<event name="configure">
  - formfactor
  - size
</event>
