# alembic
alembic node.js native plugin

## install
```
npm install alembic
```

## build
```
npm install alembic-src
```

## Features

|feature|current status|
| --------------- |---------------|
| load | o |
| save | x |
| change time | o |

|primitive|current status|
| --------------- |---------------|
| mesh | o |
| point | o |
| curve | x |
| nurbs | x |
| camera | only transform |
| material | x |
| light | x |

## Usage


put alembc_octopus for test
```
 - node_modules - umabcio - ・・・
 - alembic_octopus.abc
```

.. And test
```
D:\test>node .\node_modules\umabcio\test.js
hogehoge
{ load: [Function: load],
  get_total_time: [Function: get_total_time],
  get_time: [Function: get_time],
  set_time: [Function: set_time],
  get_object_path_list: [Function: get_object_path_list],
  get_mesh: [Function: get_mesh] }
[ '/octopus_low/octopus_lowShape' ]
{ min: 0, max: 2083 }
```

## License
MIT
