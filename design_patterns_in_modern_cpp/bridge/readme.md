The Bridge is a rather simple concept, serving as a connector or glue,
connecting two pieces together. The use of abstraction (interfaces) allows
components to interact with one another without really being aware of the
concrete implementations.
That said, the participants of the Bridge pattern do need to be aware of
each other’s existence. Specifically, a Circle needs a reference to the
Renderer and, conversely, the Renderer knows how to specifically
draw circles (thus, the name of the draw_circle() member function).
This can be contrasted with the Mediator pattern, which allows objects to
communicate without being directly aware of each other