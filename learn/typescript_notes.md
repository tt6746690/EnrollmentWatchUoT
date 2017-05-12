
# Typescript [handbook](https://www.typescriptlang.org/docs/handbook/basic-types.html)


## Types 
+ `boolean`
    + `let isDone: false = false`
+ `number`
    + `let decimal: number = 6`
+ `string`
    + `let color: string = "blue"`
    + _template string_ 
        + multiline surrounded by backtick 
        + embedded expression with `${expr}`
        + `let sentence: string = `Hello, ${name}` 
+ `array` 
    + `[]`
        + `let list: number[] = [1, 2, 3]`
    + `Array<elmType>`
        + `let list: Array<number> = [1, 2, 3]`
+ `tuple`
    + an array where type of a fixed number of elements is known, but not need to same 
    + `let x: [string, number] = ["hello", 10]`
+ `enum`
    + names to set of numeric values 
    + `enum Color {Red, Green, Blue}` 
    + `let c: Color = Color.Green`
+ `any`
    + pass compile-time check 
    + `let notSure: any = 4`
    + array of any types 
        + `let list: any[] = [1, true, "free"];`
+ `void` 
    + absence of any type 
    + return type of function
        + `function warnUser(): void {}`
    + variable with only `undefined` and `null` 
        + `let unusabl: void = undefined`
+ `undefined` and `null`
    + `--strictNullChecks`: `null` and `undfined` assignable to `void` only 
+ `never` 
+ `type assertion` 
    + `let v: any = "string"`
    + `let strlen: number = (<string>v).length`
    + `let strlen: number = (v as string).length`

## Interfaces 
+ structural subtyping 
    ```js 
    function printLabel(
        labelledObj: { label: string }){
        console.log(labelledObj.label);
    }

    // equivalent to 
    interface LabelledValue {
        label: string;
    }
    function printLabel(labelledObj: LabelledValue) {
        console.log(labelledObj.label);
    }
    ```
+ _optional properties_ 
    + suffix name with `?`
    + for 'option bags' pattern 
    ```js 
    interface SquareConfig {
        color?: string;
        width?: number;
    }

    function createSquare(config: SquareConfig): {color: string; area: number} {
        let newSquare = {color: "white", area: 100};
        if (config.color) {
            newSquare.color = config.color;
        }
        if (config.width) {
            newSquare.area = config.width * config.width;
        }
        return newSquare;
    }

    let mySquare = createSquare({color: "black"});
    ```
+ _readonly properties_ 
    ```js 
    interface Point {
        readonly x: number;
        readonly y: number;
    }
    ```
    + `ReadonlyArray<T>`
        + more strict than `const`, `.push` and even `.length` is not allowed
        + only operation allowed is read access
    + _readonly vs. const_
        + `readonly`: block property access 
        + `const`: block assignment operation 
+ _function type with interface_
    + dont think declaring function interface is necessary
    ```js 
    interface searchFunc{
        (source: string, subString: string): boolean
    }
    ```
    ```js 
    let mySearch: SearchFunc;
    mySearch = function(src: string, sub: string): boolean {
        let result = src.search(sub);
        return result > -1;
    }
    ```
+ _indexable types_ 
    + _index signature_ 
    ```js 
    interface strArray{
        [index: number]: string
    }
    let myArray: strArray
    myArray = ["bob", "fred"]
    ```
+ _class type_ 
    + both property and method can be defined
    + declaration is `public` only
    ```js 
    interface ClockInterface{
        currentTime: Date
        setTime(d: Date)
    }
    class Clock implements ClockInterface{
        currentTime: Date;
        setTime(d: Date){
            this.currentTime = d;
        }
        constructor(h: numbre, m: number){}
    }
    ```
+ _extending interface_ 
    + extending multiple interfaces allowed
    + `let square = <Square>{}` declaration with type assertion
    ```js 
    interface Shape{
        color: string;
    }
    interface Square extends Shape{
        sideLength: number;
    }
    let square = <Square>{}
    square.color = "blue"
    square.sideLength = 10
    ```


## Class 
+ class modifiers 
    + `public`
    + `private` 
    + `protected`
    ```js 
    class Animal {
        private name: string;
        constructor(theName: string) { this.name = theName; }
    }

    class Rhino extends Animal {
        constructor() { super("Rhino"); }
    }

    class Employee {
        private name: string;
        constructor(theName: string) { this.name = theName; }
    }

    let animal = new Animal("Goat");
    let rhino = new Rhino();
    let employee = new Employee("Bob");

    animal = rhino;
    animal = employee; // Error: 'Animal' and 'Employee' are not compatible
    ```
    + _mechanism_ 
        + 2 different types, regardless where they came from, if types of all member are compatible, they are compatible.
        + Comparing types with `private` and `protected` member are handled differently
            + if one is `private` the other must have a `private` member originated in the same _declaration_
            + same as `protected`
    + _observation_
        + assigning `rhino` to `animal` is fine because both share `private` side of interface from `private name: string`
        + assigning `employee` to `animal` causes error because even though `Employee` has private member called `name` it is not what we declared in `Animal`
+ `protected` 
    + act like `private` with the exception that members declared `protected` can also be accessed by instances of deriving classes
+ _readonly modifier_ 
    + readonly properties must be initialized at declaration or in constructor 
    ```js 
    class Octopus {
        readonly name: string;
        readonly numberOfLegs: number = 8;
        constructor (theName: string) {
            this.name = theName;
        }
    }
    let dad = new Octopus("Man with the 8 strong legs");
    dad.name = "Man with the 3-piece suit"; // error! name is readonly.
    ```
+ _parameter properties_ 
    + that is `readonly` can be typed in parameter of constructor
    ```js 
    class Octopus {
        readonly numberOfLegs: number = 8
        constructor(readonly name: string){}
    }
    ```
+ static, abstract, ....



## Functions 
+ _typing function_ 
    + return type not really necessary since could be inferred
    + full typing 
        ```js 
        let myAdd: (x: number, y: number)=>number = function(x: number, y: number): number { return x+y; };   
        ``` 
    + _argument type_ 
        + name given just for readability sake 
    + _return type_ 
        + after `=>`
        + use `void` if no return statement
+ _inferring types_ 
    + hence write type info on RHS
    ```js 
    // myAdd has the full function type
    let myAdd = function(x: number, y: number): number { return  x + y; };

    // The parameters 'x' and 'y' have the type number
    let myAdd: (baseValue:number, increment:number) => number =
        function(x, y) { return x + y; };
    ```
+ _optional and default parametes_ 
    + optional: `?`
        + `function buildName(firstName: string, lastName?: string){}`
    + default-initialized: 
        + is treated as optional parameter 
        + `function buildName(firstName: string, lastName = "Smith"){}`
+ _rest parameter_
    + `function buildName(firstName: string, ...restOfName: string[]) {}`



## Type Inference 
+ _places where no explicit type annotation required_ 
    + _variable initialization_ 
        + `let x = 3` infers `x: number`
+ _best common type_ 
    + considers each candidate type, picks type that is compatible with all other candidates 
    + `let x = [0, 1, null]` -> gives `number[]`
+ _contextual type_ 
    + type of expression is implied by its location 
    ```js 
    window.onmousedown = function(mouseEvent) {
        console.log(mouseEvent.buton);  //<- Error
    };
    ```
    + i.e. type of `Window.onmousedown` used to infer type of function expression
        + of course we can overwrite this with `any`



## With Jsx 
+ _conflict with type assertion_ 
    + parsing with jsx, having embedded xml, is difficult. 
        + `var foo = <foo>bar;`
        + hence, angle bracket type assertion not allowed in `.tsx`
    + solution: use `as`
        + `var foo = bar as foo`
+ _Distinguish instrinsic and user-defined elements_ 
    + convention 
        + intrinsic: begin with lowercase letter 
        + user-defined: begni with uppercase letter
            + search for identifier in scope 
+ _react integration_ 
    ```js 
    /// <reference path="react.d.ts" />

    interface Props {
    foo: string;
    }

    class MyComponent extends React.Component<Props, {}> {
    render() {
        return <span>{this.props.foo}</span>
    }
    }

    <MyComponent foo="bar" />; // ok
    <MyComponent foo={0} />; // error
    ```