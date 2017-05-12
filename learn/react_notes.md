

## Typescript 

`npm install --save react react-dom @types/react @types/react-dom`
+ `@types/` prefix gets the declaration files 



## React 

### Tutorial 

+ _JSX_ 
    + an expression and returns javascript object 
        + produces React `elements` for render in DOM
        + expression is allowed in `{}`
    + _attr_
        + _string literal_: use `"` 
            + `const element = <div tabIndex="0"></div>;`
        + _js expression_: use `{}`
            + `const element = <img src={user.avatarUrl}></img>;`
    + _children_ 
        + _empty_: enclose with `/>` 
            + `const element = <img src={user.avatarUrl} />;`
        + _with child_: 
            ```js 
            const element = (
                <div>
                    <h1>Hello!</h1> 
                    <h2>Good to see u here.</h2>
                </div>
            )
            ```
            + react-dom uses `camelCase`
    + represents `Object`
        + i.e. _React element_ used by React to construct DOM / update
        ```js 
        const element = (
            <h1 className="greeting">
                Hello
            </h1>
        )
        // Converts to (by Babel)
        const element = React.createElement(
            "h1",
            {className: "greeting"},
            "Hello"
        )
        // generates object (simplified version)
        const element = {
            type: "h1",
            props: {
                className: "greeting", 
                children: "Hello"
            }
        }
        ```
+ _Element Rendering_ 
    + _element_ 
        + smallest building block 
        + `const element = <h1> Hello </h1>;`
    + _render to DOM_ 
        ```js 
        // <div id="root"></div> somewhere 
        const element = <h1>Hello</h1>
        ReactDOM.render(
            element, 
            document.getElementById("root")
        )
        ```
    + _element is immutable_ 
        + _child_ and _attribute_ cannot be changed after creation
        ```js 
        function tick() {
        const element = (
            <div>
                <h1>Hello, world!</h1>
                <h2>It is {new Date().toLocaleTimeString()}.</h2>
            </div>
        );
        ReactDOM.render(
            element,
            document.getElementById('root')
        );
        }

        setInterval(tick, 1000);
        ```
        + DOM updates applied only necessary 
+ _Components and Props_ 
    + components are _functions_ that takes arbitrary inputs (`props`) and return React `element` 
        + convention, components are capitalized
    ```js 
    class Welcome extends React.component{
        render(){
            return <h1>Hello, {this.props.name}</h1>;
        }
    }
    ```
    + user defined element  
        + `const element = <Welcome name="Sara" />;`
        + here React passes JSX attributes (`name = "Sarah"`) to the `Welcome` component as a single object as `props`
            + `this.props`
    + _composing components_ 
        ```js
        function App() {
            return (
                <div>
                <Welcome name="Sara" />
                <Welcome name="Cahal" />
                <Welcome name="Edite" />
                </div>
            );
        }
        ```
    + _example_ 
        + comment box accepting `author`, `text`, `date` as props 
        ```js 
        function Comment(props) {
            return (
                <div className="Comment">
                    <div className="UserInfo">
                        <img className="Avatar"
                        src={props.author.avatarUrl}
                        alt={props.author.name}
                        />
                        <div className="UserInfo-name">
                        {props.author.name}
                        </div>
                    </div>
                    <div className="Comment-text">
                        {props.text}
                    </div>
                    <div className="Comment-date">
                        {formatDate(props.date)}
                    </div>
                </div>
            );
        }
        ```
        + decomposing into reusable components
        ```js 
        function Avatar(props) {
            return (
                <img className="Avatar"
                src={props.user.avatarUrl}
                alt={props.user.name}
                />
            );
        }
        ```
        ```js
        function UserInfo(props) {
            return (
                <div className="UserInfo">
                <Avatar user={props.user} />
                <div className="UserInfo-name">
                    {props.user.name}
                </div>
                </div>
            );
        }
        ```
        ```js 
        function Comment(props) {
            return (
                <div className="Comment">
                <UserInfo user={props.author} />
                <div className="Comment-text">
                    {props.text}
                </div>
                <div className="Comment-date">
                    {formatDate(props.date)}
                </div>
                </div>
            );
        }
        ```
    + _All React components must act like pure functions with respect to their props._
        + _pure function_ 
            + function always evaluates same result value given same args
            + evaluation of result does not cause observable side effect or output, such as mutation, outputs to I/O device
+ _State and lifecycle_ 
    + _state_ 
        + private and controlled by the component 
        + `this.state` is an `object`
    + _lifecycle_ 
        + class constructor initializes `this.state`
            + passing `props` to base constructor with `super(props)` is required
            + constructor called when component processed by `ReactDOM.render()`
        + _mounting_: `componentDidMount()`
            + set up when component is first rendered to DOM 
        + updating states: `this.setState()`
            + state change will trigger `render()` again, DOM updated
        + _unmounting_: `componentWillUnmount()`
            + tear down whenever DOM produced by the component is removed
    ```js 
    class Clock extends React.Component {
        constructor(props) {
            super(props);
            this.state = {date: new Date()};
        }
        componentDidMount() {
            this.timerID = setInterval(
                () => this.tick(),
                1000
            );
        }
        componentWillUnmount() {
            clearInterval(this.timerID);
        }

        tick(){
            this.setState({
                date: new Date()
            });
        }
        render() {
            return (
            <div>
                <h1>Hello, world!</h1>
                <h2>It is {this.state.date.toLocaleTimeString()}.</h2>
            </div>
            );
        }
    }
    ```
    + _updates may be async_ 
        + `this.props` and `this.state` may be updated asynchronously, 
        + should not rely on their value 
        + so use callback instead of setting states directly 
        ```js 
        // Wrong
        this.setState({
            counter: this.state.counter + this.props.increment,
        });

        // Correct
        this.setState((prevState, props) => ({
            counter: prevState.counter + props.increment
        }))
        ```
    + _date flows down (unidirectional)_ 
        + neither parent nor child components know if a component is stateful or stateless 
            + hence `state` is local/encapsulated, not accessible by any component other than one that owns it and sets it
        + although `state` can be passed down as props to child
            + the child is oblivious of whether the `props` is `state` or not
            + `<h2>It is {this.state.date.toLocaleTimeString()}.</h2>`
            + `<FormattedDate date={this.state.date}/>`
+ _Event handling_ 
    + syntax 
        ```js 
        <button onclick="activateLasers()">
            Activate Lasers
        </button>

        <button onClick={activateLasers}>
            Activate Lasers
        </button>
        ```
        + events are `camelCase` 
        + event handler is a function, rather than a string 
        + `prevenDefault()` is necessary...
        ```js 
        <a href="#" onclick="console.log('The link was clicked.'); return false">
            Click me
        </a>

        function ActionLink() {
            function handleClick(e) {
                e.preventDefault();
                console.log('The link was clicked.');
            }

            return (
                <a href="#" onClick={handleClick}>
                Click me
                </a>
            );
        }
    ```js 
    class Toggle extends React.Component {
        constructor(props) {
            super(props);
            this.state = {isToggleOn: true};

            // This binding is necessary to make `this` work in the callback
            this.handleClick = this.handleClick.bind(this);
        }

        handleClick() {
            this.setState(prevState => ({
                isToggleOn: !prevState.isToggleOn
            }));
        }

        render() {
            return (
            <button onClick={this.handleClick}>
                {this.state.isToggleOn ? 'ON' : 'OFF'}
            </button>
            );
        }
        }

        ReactDOM.render(
        <Toggle />,
        document.getElementById('root')
    );
    ```
    + Note 
        + extracting a method from object and later call that function for example in callback-based code will alter `this`
            + use `.bind(this)`
        + class methods are not bound by default
    + _solution_ 
        + use _property initializer syntax_ 
        ```js 
        class LoggingButton extends React.Component {
            // This syntax ensures `this` is bound within handleClick.
            // Warning: this is *experimental* syntax.
            handleClick = () => {
                console.log('this is:', this);
            }

            render() {
                return (
                <button onClick={this.handleClick}>
                    Click me
                </button>
                );
            }
        }
        ```
        + or use _arrow function_ in the callback in render 
        ```js 
        class LoggingButton extends React.Component {
            handleClick() {
                console.log('this is:', this);
            }

            render() {
                // This syntax ensures `this` is bound within handleClick
                return (
                <button onClick={(e) => this.handleClick(e)}>
                    Click me
                </button>
                );
            }
        }
        ```
    + `this` in React [link](http://reactkungfu.com/2015/07/why-and-how-to-bind-methods-in-your-react-component-classes/)
        + `React.Component` when rendered is instantiated by using _constructor invocation pattern_ (i.e. `new Component(...)`). 
            + since `extends React.Componnet`, `setState` and `forceUpdate` is inherited 
        + _life-cycle methods_ and `render()` are called using _method invocation pattern_ 
            + so `this` is bound to the component instance itself 
        + event handler (`onChange` or `onClick`) can come from many sources (i.e. passed down as `props` from top-level component) hence called using _function invocation pattern_
            + hence properties/members of Components (i.e. `this.state`, `this.props`, `this.setState`) is not accessible 
            + require explicity binding with `.bind()` in constructor 
            ```js 
            class InputExample extends React.Component {
                constructor(props) {
                    super(props);

                    this.state = { text: '' };
                    this.change = this.change.bind(this);
                }

                change(ev) {
                    this.setState({ text: ev.target.value });
                }

                render() {
                    let { text } = this.state;
                    return (<input type="text" value={text} onChange={this.change} />);
                }
            }
            ```
        + _Use class properties and `=>` for binding_ 
            + _class properties_ 
                + note `change` still not bound
            ```js 
            class InputExample extends React.Component {
                state = { text: '' };

                change = function(ev) {
                    this.setState({ text: ev.target.value });
                };

                // ...
            }

            // Transpiled to 
            class InputExample extends React.Component {
                constructor(...arguments) {
                    super(...arguments);

                    this.state = { text: '' };
                    this.change = function(ev) {
                    this.setState({ text: ev.target.value });
                    };
                }

                // ...
            }
            ```
            + bind `this` with `=>`
            ```js 
            change = (ev) => this.setState({ text: ev.target.value });

            // Transpiled to 
            var that = this;
            change = function(ev) { return that.setState({ text: ev.target.value }); };
            ```
        + _solution_
            ```js 
            class InputExample extends React.Component {
                state = { text: '' };
                change = ev => this.setState({text: ev.target.value});

                render() {
                    let {text} = this.state;
                    return (<input type="text" value={text} onChange={this.change} />);
                }
            }
            ```
+ _Conditional Rendering_ 
    + Rendering components depending on current `state` or `props`
        ```js 
        function UserGreeting(props) {
            return <h1>Welcome back!</h1>;
        }

        function GuestGreeting(props) {
            return <h1>Please sign up.</h1>;
        }
        function Greeting(props) {
            const isLoggedIn = props.isLoggedIn;
            if (isLoggedIn) {
                return <UserGreeting />;
            }
            return <GuestGreeting />;
        }

        ReactDOM.render(
            // Try changing to isLoggedIn={true}:
            <Greeting isLoggedIn={false} />,
            document.getElementById('root')
        );
        ```
    + _element variables_ 
        + use variable to store elements help conditionally render part of component 
        ```js
        function LoginButton(props) {
            return (
                <button onClick={props.onClick}>
                Login
                </button>
            );
        }
        function LogoutButton(props) {
            return (
                <button onClick={props.onClick}>
                Logout
                </button>
            );
        }

        // LoginControl is stateful 
        class LoginControl extends React.Component {

            state = {isLoggedIn: false}
            handleLoginClick = () => {
                this.setState({isLoggedIn: true})
            }
            handleLogoutClick = () => {
                this.setState({isLoggedIn: tre})
            }

            render() {
                const isLoggedIn = this.state.isLoggedIn;

                let button = null;
                if (isLoggedIn) {
                    button = <LogoutButton onClick={this.handleLogoutClick} />;
                } else {
                    button = <LoginButton onClick={this.handleLoginClick} />;
                }

                return (
                <div>
                    <Greeting isLoggedIn={isLoggedIn} />
                    {button}
                </div>
                );
            }
        }

        ReactDOM.render(
            <LoginControl />,
            document.getElementById('root')
        );
        ```
    + _inline if and logical `&&` operator_ 
        + `true && expression` evaluates to `expression`, can be used in `{}` in jsx
        ```jsx
        function Mailbox(props) {
            const unreadMessages = props.unreadMessages;
            return (
                <div>
                <h1>Hello!</h1>
                {unreadMessages.length > 0 &&
                    <h2>
                    You have {unreadMessages.length} unread messages.
                    </h2>
                }
                </div>
            );
        }

        const messages = ['React', 'Re: React', 'Re:Re: React'];
        ReactDOM.render(
            <Mailbox unreadMessages={messages} />,
            document.getElementById('root')
        );
        ```
    + _inline if and conditional operator_ 
        ```js 
        render(){
            const isLoggedIn = this.state.isLoggedIn
            return (
                <div>
                    this user <b>{isLoggedIn ? 'current': 'not'}</b> logged in.
                </div>
            )
        }
        ```
    + _prevent component from rendering_    
        + component is not rendered if return `null` instead of jsx elements
        ```js 
        function WarningBanner(props){
            if(!props.warn){
                return null
            }
            return (
                <div className = "warning">
                    Warning 
                </div>
            )
        }

        class Page extends React.component(){
            state = {showWarning:  true}
            handleToggleClick = () => {
                this.setState(prevState => ({
                    showWarning: !prevState.showWarning
                }))
            }

            render(){
                return (
                    <div> 
                        <WarningBanner warn={this.state.showWarning} .>
                        <button onClick={this.handleToggleClick}>
                            {this.state.showWarning ? 'Hide': 'Show'}
                        </button>
                    </div>
                )
            }
        }

        ReactDOM.render(
            <Page />
            document.getElementById('root')
        )
        ```
+ _Lists and Keys_ 
    + _rendering multiple components with `map()`_
        + a collection of element can be built by using `{l}` in jsx where `l: element[]`
        ```js 
        const numbers = [1, 2, 3, 4, 5]
        const listItems = numbers.map(x => 
            <li>{x}</li>
        )

        ReactDOM.render(
            <ul>{listItems}</ul>,
            document.getElementById("root")
        )
        ```
    + _basic list component_ 
        + Note `key: string` attributes is required for creating lists of elements
        ```js 
        function NumberList(props){
            const numbers = props.numbers;
            cosnt listItems = numbers.map((x) => 
                <li key={x.toString()}>{x}</li>
            );
            return (
                <ul>{listItems}</ul>
            )
        };

        ReactDOM.render(
            <NumberList numbers={[1, 2, 3]} />,
            document.getElementById("root")
        );
        ```
    + _keys_ 
        + helps React identify which item has changed, are added, or are removed 
        + uniquely given to elements in list (i.e. unique amongst siblings)
        ```js 
        const todoItems = todos.map((todo) => 
            <li key={todo.id}>
                {todo.text}
            </li>
        )
        ```
    + _extracting component with key_ 
        + specify `key` only in context of surrounding `array`
        ```js 
        function ListItem(props) {
            // Correct! There is no need to specify the key here:
            return <li>{props.value}</li>;
        }

        function NumberList(props) {
            const numbers = props.numbers;
            const listItems = numbers.map((number) =>
                // Correct! Key should be specified inside the array.
                <ListItem key={number.toString()}
                        value={number} />
            );
            return (
                <ul>
                {listItems}
                </ul>
            );
        }

        const numbers = [1, 2, 3, 4, 5];
        ReactDOM.render(
            <NumberList numbers={numbers} />,
            document.getElementById('root')
        );
        ```
    + _note keys is not passed down_ 
        + use another attribute `props.id` to pass down attributes
        ```js 
        const content = posts.map((post) =>
            <Post
                key={post.id}
                id={post.id}
                title={post.title} />
        );
        ```
+ _Forms_  
    + _comparison_
        + HTML 
            + form element keep some internal state, browse to new page upon form submission 
            + i.e. `<input>`, `<textarea>`, `<select>` maintain their own state and update based on user input 
             + however, its better to use js function to handle submission, and access to data user entered to form
        + React 
            + mutable state kept in `this.state` and updated with `setState()`
    + _controlled components_ 
        + component that renders the form
            + stores `state` of input
                + `state.value`
            + update `state` 
                + `handleChange` updates `state.value`
            + controls display 
                + `<input value={this.state.value}/>`
        + `input`
        ```js 
        class NameForm extends React.Component{
            state = { value: '' }
            handleChange = (e) => {
                this.setState({ value: e.target.value })
            }
            handleSubmit = (e) => {
                alert(`A name was submitted: ${this.state.value}`)
                e.preventDefault()
            }

            render(){
                return (
                    <form onSubmit={this.handleSubmit}>
                        <label>
                            Name: 
                            <input type="text" value={this.state.value} onChange={this.handleChange}/>
                        </label>
                        <input type="submit" value="Submit"/>
                    </form> 
                )
            }
        } 
        ```
        + `textarea`
        ```js 
        class EssayForm extends React.Component {
            constructor(props) {
                super(props);
                this.state = {
                value: 'Please write an essay about your favorite DOM element.'
                };

                this.handleChange = this.handleChange.bind(this);
                this.handleSubmit = this.handleSubmit.bind(this);
            }

            handleChange(event) {
                this.setState({value: event.target.value});
            }

            handleSubmit(event) {
                alert('An essay was submitted: ' + this.state.value);
                event.preventDefault();
            }

            render() {
                return (
                <form onSubmit={this.handleSubmit}>
                    <label>
                    Name:
                    <textarea value={this.state.value} onChange={this.handleChange} />
                    </label>
                    <input type="submit" value="Submit" />
                </form>
                );
            }
        }
        ```
        + `select`
            + In React, instead of using `selected` attribute, a `value` attribute on root `select` tag controls the selected row 
            ```js 
            <select>
                <option value="grapefruit">Grapefruit</option>
                <option value="lime">Lime</option>
                <option selected value="coconut">Coconut</option>
                <option value="mango">Mango</option>
            </select>
            ```
        ```js 
        class FlavorForm extends React.Component {
            constructor(props) {
                super(props);
                this.state = {value: 'coconut'};

                this.handleChange = this.handleChange.bind(this);
                this.handleSubmit = this.handleSubmit.bind(this);
            }

            handleChange(event) {
                this.setState({value: event.target.value});
            }

            handleSubmit(event) {
                alert('Your favorite flavor is: ' + this.state.value);
                event.preventDefault();
            }

            render() {
                return (
                <form onSubmit={this.handleSubmit}>
                    <label>
                    Pick your favorite La Croix flavor:
                    <select value={this.state.value} onChange={this.handleChange}>
                        <option value="grapefruit">Grapefruit</option>
                        <option value="lime">Lime</option>
                        <option value="coconut">Coconut</option>
                        <option value="mango">Mango</option>
                    </select>
                    </label>
                    <input type="submit" value="Submit" />
                </form>
                );
            }
        }
        ```
+ _lifting state up_ 
    + State change may be reflect in multiple component, 
        + lift state to their closest common ancestor
    + should rely on _top-down data flow_ 
        + top-level state passed down as props 
        + bottom-level event passed up to change state and re-render
    ```js 
    import * as React from "react"
    import * as ReactDOM from "react-dom"


    const scaleNames = {
        c: 'Celsius',
        f: 'Fahrenheit'
    }

    function BoilingVerdict(props){
        if(props.celsius >= 100){
            return <p>The water would boil</p>
        }
        return <p>The water would not boil</p>
    }

    function toCelsius(fahrenheit: number){
        return (fahrenheit - 32) * 5 / 9
    }
    function toFahrenheit(celsius:  number){
        return (celsius * 9 / 5) + 32
    }

    class TemperatureInput extends React.Component<any, any> {
        // state = { temperature: '' }
        handleChange = (e) => {
            // this.setState({ temperature: e.target.value })
            this.props.onTemperatureChange(e.target.value)
        }
        render() {
            // const temperature = this.state.temperature
            const temperature = this.props.temperature
            const scale = this.props.scale
            return (
                <fieldset>
                    <legend>Enter temperature in {scaleNames[scale]}</legend>
                    <input value={temperature} onChange={this.handleChange} />
                </fieldset>
            )
        }

    }


    // owns shared state for c and f temperature 
    class Calculator extends React.Component<any, any> {

        state = {
            temperature: '',
            scale: 'c'
        }

        handleCelsiusChange = (temperature) => {
            this.setState({scale: 'c', temperature})
        }
        handleFahrenheitChange = (temperature) => {
            this.setState({ scale: 'f', temperature })
        }


        render(){

            const scale = this.state.scale 
            const temperature = this.state.temperature
            const celsius = scale === 'f' ? tryConvert(temperature, toCelsius): temperature
            const fahrenheit = scale === 'c' ? tryConvert(temperature, toFahrenheit) : temperature

            return (
                <div>
                    <TemperatureInput 
                        scale="c"
                        temperature={celsius}
                        onTemperatureChange={this.handleCelsiusChange}/>
                    <TemperatureInput
                        scale="f"
                        temperature={fahrenheit}
                        onTemperatureChange={this.handleFahrenheitChange} />
                </div>
            )
        }
    }


    function tryConvert(temperature: string, convert: (number)=>number){
        const input = parseFloat(temperature)
        if(isNaN(input)){
            return '';
        }
        const output = convert(input)
        const rounded = Math.round(output * 1000) / 1000 
        return rounded.toString()
    }


    ReactDOM.render(
        <Calculator />,
        document.getElementById("app")
    )
    ```
+ _Composition vs. Inheritance_  
    + _containment_ 
        + use case
            + `SideBar` or `Dialog` where _children_ not known before hand 
        + solution 
            + `props.children` pass children elements directly into their output 
        ```js 
        function FancyBorder(props){
            return (
                <div className={`FancyBorder FancyBorder-${props.color}`}>
                    {props.children}
                </div>
            )
        }

        function WelcomeDialog(){
            return (
                <FancyBorder color="blue">
                    <h1 className="Dialog-title">
                        Welcome
                    </h1>
                    <p className="Dialog-message">
                        Thank u for visiting spacecraft
                    </p>
                </FancyBorder>
            )
        }
        ```
        + Anything inside of `<FancyBorder>` passed into `FancyBOrder` Component as `props.children`
        ```js 
        function SplitPane(props) {
            return (
                <div className="SplitPane">
                <div className="SplitPane-left">
                    {props.left}
                </div>
                <div className="SplitPane-right">
                    {props.right}
                </div>
                </div>
            );
        }

            function App() {
            return (
                <SplitPane
                left={
                    <Contacts />
                }
                right={
                    <Chat />
                } />
            );
        }
        ```
        + If requires multiple placeholder, may need to come up with ur own convention insteads
+ _Thinking in React_ 
    + steps 
        + break UI into component hierarchy
        + building a static version 
            + use a lot of `props` and dont use `state`
        + identify minimal (complete) representation of UI state
            + change trigger is reflected in updating `state`
            + minimal set of mutable state?
            + checking if using `state` is not appropriate 
                + passed as `props` from parent 
                + remain unchanged 
                + can be computed based on other `state`/`props`
        + determine where `state` shoud live 
            + steps 
                + find components requiring render based on `state`
                + find a common owner component 
            + idea 
                + maintain a top-level `state` 
                + pass down handler for changing `state` as `props` to lower level components 
                + `state` change initiated at lower level components and propagate back up to change the top-level `state`
        + add inverse data flow 
+ _JSX in depth_ 
    + a syntax sugar 
        + `React.createElement(component, props, ...children)`
+ _Reconciliation_  
    + _diffing algo_
        + `O(n^3)` for tree transformation 
        + `O(n)` based on heuristics 
            + two elements of different type produce different tres 
                + _Element of different type_ 
                    + old DOM destroyed 
                    + `componentWillUnmount()` 
                    + `componentWillMount()`
                    + `componentDidMount()`
                    + `state` is destoyed 
                    + `render()`
                + _Element of same type_ 
                    + updates attributes 
                    + state is maintained 
                    + `render()`
                 + _component elements of same type(i.e. list)_
                    + `state` is maintained 
                    + `props` updated with 
                        + `componentWillReceiveProp()`
                        + `componentWillUpdate()`
                    + `render()`
                    +  however, inserting a new element say `<li>` at beginning of list has worse performace -> use `key`
            + hint at stable child elements with `key` 
                + use `key` to match original tree in subsequent trees
                + `key` should be determinstic, stable, predictable unique
        


        

       
        
