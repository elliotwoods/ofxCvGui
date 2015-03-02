### My projects won't load

Note that ofxCvGui2 is a little bit funny about its folder location (sorry!). It actually needs to be installed as `openFrameworks/addons/ofxCvGui` not `openFrameworks/addons/ofxCvGui<u>2</u>`.

If you have the `2` in the addon path name, then you're likely to come across a host of errors, e.g.:

* Projects in Visual Studio refuse to load because they reference an incorrect path
* Assets folder is not copied
* Other addons cannot include ofxCvGui

### My mouse pressed action does not fire

This could be perhaps because your element bounds are not set (perhaps even somewhere up the tree of ElementGroup's, etc).

### What is the correct pattern for `MouseArguments::takeMousePress()`?

I suggest checking out some of the Widget classes (e.g. `Widgets::Toggle`) to see how mouse press taking is implemented. 

Generally, if you want to take a mouse press:

1. If only a portion of your Element is clickable, then first check to see if the mouse coordinates are in the clickable region.
2. Call `args.takeMousePress()` and check the result. If it's false then you haven't been able to take the mouse press (either it's not down, it's not in your bounds, or it is already taken).
3. If you were able to take the mouse press, then perform your action. Also during the next frame your Element base class will report `getMouseState() == LocalMouseState::Down`

Note : Taking a mouse press is not mouse button dependant.

### When should I respond to a dragged or released action?

You will be passed any dragged or released actions which belong to you or any of your children. If you have no children then it's safe to presume the action is yours, if you do have children which you manage, then first check whether it's you who owns the mouse click (using `args.getOwner()`).