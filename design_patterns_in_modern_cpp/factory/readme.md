A factory method is a class member that is used for creating an object. It
typically replaces a constructor.
A factory is typically a separate class that knows how to construct
objects, though, if you pass a function (as in function or similar) that
constructs objects, this argument is also called a factory.
An abstract factory is, as its name suggests, an abstract class that can be
inherited by concrete classes that offer a family of types. Abstract
factories are rare in the wild.
A factory has several critical advantages over a constructor call,
namely:
A factory can say no, meaning that instead of actually returning an
object, it can return, for example, a default-initialized smart pointer, an
optional<T> or a nullptr.
A factory method can be polymorphic, so it can return a parent
class/interface, or a pointer thereof. It can also support the return of
different data types using other means, for example, via a variant.
Naming is better and unconstrained, unlike the constructor name. You
can call the factory methods whatever you want.
A factory can implement caching and other storage optimizations; it is
also a natural choice for approaches such as pooling or the Singleton
pattern (more on this later).
Factories can be used to encapsulate other concerns (as per Separation of
Concerns) such as validation.
