# Actions

## Mouse

|                                   	| `Action::Pressed` 	| `Action::Dragged`                    	| `Action::Released`                                           	| `Action::DoubleClick` 	| `Action::Move`   	|
|-----------------------------------	|-------------------	|--------------------------------------	|--------------------------------------------------------------	|-----------------------	|------------------	|
| `LocalMouseState::Waiting`        	| [See notes below] 	|                                      	| None                                                         	| Notify listeners      	| Notify listeners 	|
| `LocalMouseState::Down`           	| N/A               	| Set `localMouseState` to `Dragging`. 	| (1) Notify listeners. (2) Set `localMouseState` to `Waiting` 	| N/A                   	| N/A              	|
| `LocalMouseState::Dragging`       	| N/A               	| Notify listeners                     	| (1) Notify listeners. (2) Set `localMouseState` to `Waiting` 	| N/A                   	| N/A              	|
| `LocalMouseState::ChildOwnsMouse` 	| N/A               	| Notify listeners                     	| (1) Notify listeners. (2) Set `localMouseState` to `Waiting` 	| N/A                   	| N/A              	|

### `Action::Pressed` and `LocalMouseState::Waiting`

If hit:
1. Notify listeners (top first).
2. If we have `onMouseReleased` listeners, then try to take the cursor.
3. If cursor taken (either by listeners or otherwise) then notify upstream who has taken the cursor.
4. If we have taken the cursor then change our `localMouseState` to `Down`.
5. If child has taken the cursor, then set `localMouseState` to `ChildOwnsMouse`

# Widgets

## Slider

When the value is changed:
* Firstly call validators
* Notify any remaining listeners