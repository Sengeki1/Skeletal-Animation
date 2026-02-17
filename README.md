When you load a model using Assimp:

```cpp
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path, flags);
```

You get:

```bash
  aiScene
   ├── aiNode (hierarchy tree)
   ├── aiMesh[]
   │    ├── vertices
   │    ├── bones[]
   |    ├── ...
   ├── aiAnimation[]
```

The `aiScene` property contains sub-properties such as:

* **aiNode** which represent the bone hierarchy:

```bash
  Root
   └── Spine
        └── Arm
             └── Hand
```
if the spine moves → `Arm` and `Hand` move too and each `aiNode` has the following properties:

```cpp
  {
      aiString mName;
      aiMatrix4x4 mTransformation; // local bone transformations
      aiNode* mParent;
      aiNode** mChildren;
  }
```

* **aiMesh** stores mesh data such as:

```cpp
  ...
  aiMesh->mBones[]
```

and each bone has the following property:

```cpp
  {
      aiString mName;
      aiMatrix4x4 mOffsetMatrix;
      aiVertexWeight* mWeights;
  }
```

A vertice is influenced by the weights of a bone and the sum of weights that influence the vertice must sum to `1` meaning a vertice can be influenced by multiple bones and the sum of all the weights of all the bones must be `1`.

| Vertex | Bone | Weight |
| ------ | ---- | ------ |
| 12     | Arm  | 0.7    |
| 12     | Hand | 0.3    |

That vertex is 70% Arm, 30% Hand.

**Note:** If all weights = 0 → vertex collapses to origin.

