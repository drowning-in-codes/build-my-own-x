fn create_box() {
    let _box1 = Box::new(3i32);
}
#[derive(PartialEq, PartialOrd)]
struct Centimeters(f64);

struct Foo {
    x: (u32, u32),
    y: u32,
}
struct Baz(u32, u32);
enum Bar {
    A(u32, u32),
    B(u32, u32),
    C,
}
#[derive(Debug, Clone)]
struct Point(i32, i32);
fn left_most<'a>(p1: &'a Point, p2: &'a Point) -> Point {
    if p1.0 <= p2.0 {
        p1.clone()
    } else {
        p2.clone()
    }
}
fn main() {
    let p1: Point = Point(10, 20);
    let p2 = Point(10, 20);
    let p3 = left_most(&p1, &p2);
    println!("({}, {})", p1.0, p1.1);

    let foo = Foo { x: (1, 2), y: 3 };
    let baz = Baz(1, 2);
    let a = Bar::A(1, 2);
    match a {
        Bar::A(1, b) => {
            println!("a: (1, {})", b);
        }
        Bar::A(a, b) => {
            println!("a: ({}, {})", a, b);
        }
        _ => {}
    }
    match baz {
        Baz(1, b) => {
            println!("baz: (1, {})", b);
        }
        Baz(a, b) => {
            println!("baz: ({}, {})", a, b);
        }
    }
    match foo {
        Foo { x: (1, b), y } => {
            println!("x: (1, {}), y: {}", b, y);
        }
        Foo { x, y } => {
            println!("c-struct");
            println!("x: ({}, {}), y: {}", x.0, x.1, y);
        }
    }
    let t = (1, 2, 3);
    println!("{:?}", t);

    let x = (1, 2);
    let _box2 = Box::new(5i32);
    {
        let _box3 = Box::new(4i32);
    }
}
