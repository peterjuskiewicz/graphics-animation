# graphics-animation

## Shape building

I've used GL_LINE_STRIP, GL_TRIANGLES, GL_LINES, GL_POLYGON to build shapes of human, aquarium, fish, lamp, circle, rain and window.

## Movement

To animate objects I've used glTranslatef, glRotatef methods encapsuled in glPushMatrix and glPopMatrix methods.


## Morphism

To change the shape of object (fish) I've used array to define coordinates of fish before and after mutation and used tween method to gradualy morph the object.

## Scenes

There are 4 scenes in the animation and the change of scenes is triggered by ceirtan events such as position of objects or counters

## Particle systems

I've used and modified the code provided by the tutor to generate rain, spray and explosion effects for the purpose of my project.

## Viewports

There are 2 viewports defined by adding glViewport method: window and inside of the room

## Double buffering

The animation uses double buffering by adding glutSwapBuffers at the end of the display function and changing glutInitDisplayMode to GLUT_DOUBLE

## Drawing text

I've used renderSpacedBitmapString method to draw the final ad message on the display

## Misc

The lightbulb is changing colours giving flashing effect, array has been use to store the values of colours.





