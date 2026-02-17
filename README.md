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
