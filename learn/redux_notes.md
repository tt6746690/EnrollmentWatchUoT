
## Networking with React 
+ _approaches_ 
    + root component 
        + store ajax response in `state`
        + pass `state` down as `props`
    + container component 
    + redux async action 
    + relay + graphQL



## Redux 
+ _motivation_ 
    + complicated state management
        + i.e. server responses, cached data, locally created data\
        + UI state 
    + _solution_: make state mutation predictable
+ _core concepts_ 
    ```js 
    {
        todos: [{
            text: 'Eat food',
            completed: true
        }, {
            text: 'Exercise',
            completed: false
        }],
        visibilityFilter: 'SHOW_COMPLETED'
    }
    ``` 
    + `state` 
        + a plain object,
        + representing the _model_, but no setters 
    ```js 
    { type: 'ADD_TODO', text: 'Go to swimming pool' }
    { type: 'TOGGLE_TODO', index: 1 }
    { type: 'SET_VISIBILITY_FILTER', filter: 'SHOW_ALL' }
    ```
    + `action` 
        + a javascript object
        + dispatched to change `state`
    ```js 
    function visibilityFilter(state = 'SHOW_ALL', action) {
        if (action.type === 'SET_VISIBILITY_FILTER') {
            return action.filter;
        } else {
            return state;
        }
    }

    function todos(state = [], action) {
        switch (action.type) {
        case 'ADD_TODO':
            return state.concat([{ text: action.text, completed: false }]);
        case 'TOGGLE_TODO':
            return state.map((todo, index) =>
            action.index === index ?
                { text: todo.text, completed: !todo.completed } :
                todo
        )
        default:
            return state;
        }
    }
    function todoApp(state = {}, action) {
        return {
            todos: todos(state.todos, action),
            visibilityFilter: visibilityFilter(state.visibilityFilter, action)
        };
    }
    ```
    + `reducer` 
        + a function that takes `state` and `action`, and return the next `state` of app
        + each reducer manage part of state
        + a top level reducer that manages the complete state of app by calling other reducers
    + basically describes
        + how `state` is updated in response to `action`
+ _three principles_ 
    + _single source of truth_ 
        + the `state` of the whole app is stored in an object tree within a single `store`
        + `store.getState()` 
    + _`state` is readonly_
        + the only way to change `state` is to emit an `action`, an object describing what happened
            + view/callbacks never change state; but express intent to change state 
        + prevent race condition
        ```js 
        store.dispatch({
            type: 'COMPLETE_TODO',
            index: 1
        })
        ```
    + _changes are made with pure functions_ 
        + _state tree_ transformation is achieved by pure `reducers`
            + takes in `state` and `action` and return the next `state`
            + no mutation allowed
            ```js 
             function visibilityFilter(state = 'SHOW_ALL', action) {
                switch (action.type) {
                    case 'SET_VISIBILITY_FILTER':
                    return action.filter
                    default:
                    return state
                }
            }

            function todos(state = [], action) {
                switch (action.type) {
                    case 'ADD_TODO':
                    return [
                        ...state,
                        {
                        text: action.text,
                        completed: false
                        }
                    ]
                    case 'COMPLETE_TODO':
                    return state.map((todo, index) => {
                        if (index === action.index) {
                        return Object.assign({}, todo, {
                            completed: true
                        })
                        }
                        return todo
                    })
                    default:
                    return state
                }
            }

            import { combineReducers, createStore } from 'redux'
            let reducer = combineReducers({ visibilityFilter, todos })
            let store = createStore(reducer)
            ```
+ _prior art_ 
    + _flux_ 
        + redux concentrates state transformation instead of letting application code modifying the data
        + redux does not have dispatchers 
        + redux assumes data never mutates; always return new object, 
            + easy with spread operator 
            + or Immutable
+ _Action_ 
    + payloads of info sending data from app to store
        + js object 
        + `type`: string constants 
        ```js 
        import { ADD_TODO, REMOVE_TODO } from '../actionTypes'
        {
            type: ADD_TODO,
            text: 'Build my first Redux app'
        }
        ```
    + _action creator_  
        + function that returns an action
        ```js 
        function addTodo(text) {
            return {
                type: ADD_TODO,
                text
            }
        }
        dispatch(addTodo(text))
        ```
    + _bound action creator_ 
        + automatic dispatch 
        + `const boundAddTodo = (text) => dispatch(addTodo(text)`
    ```js 
    /*
    * action types
    */

    export const ADD_TODO = 'ADD_TODO'
    export const TOGGLE_TODO = 'TOGGLE_TODO'
    export const SET_VISIBILITY_FILTER = 'SET_VISIBILITY_FILTER'

    /*
    * other constants
    */

    export const VisibilityFilters = {
        SHOW_ALL: 'SHOW_ALL',
        SHOW_COMPLETED: 'SHOW_COMPLETED',
        SHOW_ACTIVE: 'SHOW_ACTIVE'
    }

    /*
    * action creators
    */

    export function addTodo(text) {
    return { type: ADD_TODO, text }
    }

    export function toggleTodo(index) {
    return { type: TOGGLE_TODO, index }
    }

    export function setVisibilityFilter(filter) {
    return { type: SET_VISIBILITY_FILTER, filter }
    }
    ```
+ _reducer_ 
    + specify how app's state change in response
    + `state` is stored as a single object, 
        + design a good shape requires 
        + stoers both data and UI state, keep them separate
    + pure `(prevState, action => newState)`
        + disallowed 
            + argument mutation 
            + side effects like API calls and routing transition 
            + call non-pure function (i.e. `Date.now()` or `math.random()`)
    ```js 
    function todoApp(state, action) {
        if (typeof state === 'undefined') {
            return initialState
        }
        return state
    }
    // OR use default arg 
    function todoApp(state = initialState, action) {
        switch (action.type) {
            case SET_VISIBILITY_FILTER:
            return Object.assign({}, state, {
                visibilityFilter: action.filter
            })

            // Or 

            return {...state, visibilityFilter: action.filter}

            default:
                return state
        }
    }
    ```
    + _observation_ 
        + `state` is undefined for the first time, so chance to return initial state of app 
        + note `Object.assign()` do not mutate but instead assign to a new `{}`
        + default previous `state` returned
        + `...` object spread operator returns shallow copy ( so arrays can be mutated )
    ```js 
    function todoApp(state = initialState, action){
        switch(action.type){
            case actions.SET_VISIBILITY_FILTER:
                return {...state, visibilityFilter: action.filter}
            case actions.ADD_TODO:
            case actions.TOGGLE_TODO:
                return {...state, todos: todos(state.todos, action)}
        }

    }
    function todos(state = [], action) {
        switch (action.type) {
            case actions.ADD_TODO:
                return [
                        ...state,
                        {
                            text: action.text,
                            completed: false
                        }
                    ]
            case actions.TOGGLE_TODO:
                return state.map((todo, index) => {
                        if(index === action.index){
                            return {...todo, completed: !todo.completed}
                        }
                        return todo
                    })
            default:
                return state
        }
    }
    ```
    + _reducer composition_ 
        + modifying nested `state` object utilizing other reducers
        + each reducer is managing its own part of global `state`, the `state` parameter is different for every `reducer`, and corresponds to part of state it manages 
        + easier composition with `combineReducers()`
            + generates a combined function that calls reducers with the _slices of `state` according to their keys_ and combining their result into a single object again 
        ```js 
        import { combineReducers } from 'redux'
        const todoApp = combineReducer({
            visibilityFilter,
            todos
        })

        // equivalent to 
        export default function todoApp(state = {}, action) {
            return {
                visibilityFilter: visibilityFilter(state.visibilityFilter, action),
                todos: todos(state.todos, action)
            }
        }
        ```
        + Note `key` of object into `combineReducer` has to match `key` of `state`, however we can assign a different function name 
            ```js 
            const reducer = combinedReducers({
                a: doSomethingWithA,
                b: processB,
                c: c
            })
            ```
        + _top level reducer_ 
            ```js 
            import { combineReducers } from 'redux'
            import * as reducers from './reducers'

            const todoApp = combineReducers(reducers)
            ```
    ```js 
    import * as actions from "./actions"
    import { combineReducers } from 'redux'

    declare interface ObjectConstructor {
        assign(...objects: Object[]): Object;
    }


    const { SHOW_ALL }  = actions.VisibilityFilters
    function visibilityFilter(state = SHOW_ALL, action){
        switch(action.type){
            case actions.SET_VISIBILITY_FILTER:
                return action.filter 
            default: 
                return state 
        }
    }

    function todos(state = [], action) {
        switch (action.type) {
            case actions.ADD_TODO:
                return [
                        ...state,
                        {
                            text: action.text,
                            completed: false
                        }
                    ]
            case actions.TOGGLE_TODO:
                return state.map((todo, index) => {
                        if(index === action.index){
                            return {...todo, completed: !todo.completed}
                        }
                        return todo
                    })
            default:
                return state
        }
    }

    const todoApp = combineReducers({
        visibilityFilter,
        todos
    })

    export default todoApp
    ```
+ _store_ 
    + an object that 
        + holds application `state`
        + allow access to `state` via `getState()`
        + allow state update with `dispatch(action)`
        + register listeners via `subscribe(listener)`
        + handles unregistering of listens via a function returned by `subscribe(listener)`
    + only one `store` needed 
    + create `store` with `createStore` 
        + second arg is initial `state`
    ```js 
    import { createStore } from 'redux'
    import todoApp from './reducers'
    let store = createStore(todoApp)


    // Every time the state changes, log it
    // Note that subscribe() returns a function for unregistering the listener
    let unsubscribe = store.subscribe(() =>
    console.log(store.getState())
    )

    // Dispatch some actions
    store.dispatch(addTodo('Learn about actions'))
    store.dispatch(addTodo('Learn about reducers'))
    store.dispatch(addTodo('Learn about store'))
    store.dispatch(toggleTodo(0))
    store.dispatch(toggleTodo(1))
    store.dispatch(setVisibilityFilter(VisibilityFilters.SHOW_COMPLETED))

    // Stop listening to state updates
    unsubscribe()
    ```
+ _data flow_ 
    + _strict unidirectional data flow_ 
    + _lifecycle_ 
        1. `store.dispatch(action)`
            + usually in XHR callbacks , or even at scheduled intervals 
        2. `reducer(state, action) => state`
            + redux store calls reducer function given, outputs next state 
            + no side effects, no API calls or router transitions
        3. `rootReducer = combineReducer(reducers)`
            + root reducer combine output of multiple reducer into a single _state tree_
        4. redux `store` saves complete state tree by root reducer 
            + new tree -> next `state`
            + every `listener` registered with `store.subscribe(listener)` will be invoked now.
                + `listener` may call `store.getState()` to get current state 
            + `setState(newState)` 
            + `render()` invoked
                + UI updated to reflect new state
+ _connect to react_
    + _key concept_ 
        + React works well with redux because react describe UI as a function of `state`, and Redux emits `state` updates in response to `action`
    + _presentation vs. container component_ 
        + _presentational_ 
            + how things look 
            + not aware of redux 
            + read data from `props`
            + invoke callback from `props`
        + _container component_ 
            + how things work (data fetching, state updates)
            + aware of redux
            + subscribe to Redux state 
            + dispatch Redux action 
            + usually generated by react redux 
    + _design_ 
        + presentational 
            + `TodoList` shows a list of todos, 
        + container
            + `VisibleTodoList` 
                + subscribes to Redux store and knows how to apply current visibility filter
            + `FilterLink`
                + gets current visibility filter and renders `TodoList`
    + _impl container components_ with `connect()`
        + A react componnet that uses `store.subscribe()` to read part of Redux state tree ands upply props to a presentational components it renders
        + `store.dispatch()`
    + `connect()`
        + `mapStateToProps`: how to transform current Redux store `state` into `props` that is to be passed onto presentational containers 
            + determines how UI updates its view
        + `mapDispatchToProps()`: takes `dispatch()` and returns callback `props` to be injected to presentational containers 
            + specifies `action` from user input
            + triggers `dispatch(action)`
    + `store`
        + passed to _container components_ so that they can be subscribed to
        + use `<Provider>` to make store available to all container components in application without passing it explicitly 
            + need to use it once only 
+ [complete todolist example](http://redux.js.org/docs/basics/ExampleTodoList.html)
        


## Redux with async
+ read later 

