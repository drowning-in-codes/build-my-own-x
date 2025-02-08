The goal of the Builder pattern is to define a component dedicated entirely to piecewise
construction of a complicated object or set of objects. We have observed the following key
characteristics of a Builder:

1. Builders can have a fluent interface that is usable for complicated construction using a single
   invocation chain. To support this, builder functions should return this or \*this.
2. To force the user of the API to use a Builder, we can make the target object’s constructors
   inaccessible and then define a static create() function that returns the builder.
3. A builder can be coerced to the object itself by defining the appropriate operator.Groovy-style builders are possible in C++ thanks to uniform initializer syntax. This approach
   is very general and allows for the creation of diverse DSLs.
4. A single builder interface can expose multiple sub-builders. Through clever use of inheritance
   and fluent interfaces, one can jump from one builder to another with ease.
