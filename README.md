# Skeletal Animation

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
if the spine moves → `Arm` and `Hand` move too. And each `aiNode` has the following properties:

```cpp
  {
      aiString mName;
      aiMatrix4x4 mTransformation; // local bone transformations (default pose)
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

```cpp
  {
      unsigned int mVertexId;
      float mWeight;
  }
```

| Vertex | Bone | Weight |
| ------ | ---- | ------ |
| 12     | Arm  | 0.7    |
| 12     | Hand | 0.3    |

That vertex is 70% Arm, 30% Hand.

**Note:** If all weights = 0 → vertex collapses to origin.

When the model is also imported, the mesh is already in its bind pose meaning that the default pose created in blender for each bone, already has some transformations answering the question: "*How do i move a vertex from model space into this bone's local space?*" because during animation, bones move relative to their bind pose.

Model Space → Bone Local Space

This is applied by using the offset matrix of the given bone and its simply the inverse of it's global transformations:

  <p align="center">
    <b>Offset = Inverse(BindPoseGlobalTransform)</b>
  </p>

When the model is exported the mesh is already in bind pose which means bones already had transforms applied. So vertices are already positioned correctly. But when animating, we want only the NEW movement applied not the old bind pose movement again. If we just apply animated transform directly the vertex gets transformed twice and that’s wrong.

Offset removes the original bind pose transformation of that bone in other words it “undoes” the bind pose. As example, imagine in bind pose, elbow bone was translated 5 units right. So bind pose matrix is:

  <p align="center">
    <b>Bind = T(5,0,0)</b>
  </p>

Offset becomes:

  <p align="center">
    <b>Offset = inverse(T(5,0,0)) = T(−5,0,0)</b>
  </p>

Offset moves vertices 5 units left. Why? Because we want to temporarily bring the vertex into bone local space.

* **aiAnimatation**

This is the animation data where we will work with at runtime:

```cpp
  {
    double mDuration;
    double mTicksPerSecond;
    aiNodeAnim** mChannels;
  }
```

and each channel has:

```cpp
  {
    aiVectorKey* mPositionKeys;
    aiQuatKey* mRotationKeys;
    aiVectorKey* mScalingKeys;
    unsigned int mNumPositionKeys;
    unsigned int mNumRotationKeys;
    unsigned int mNumScalingKeys;
  }
```

To interpolate between each keyframe we simply use the result of the `currentTime` of the application multiplied by the `mTicksPerSecond` where we converts seconds to ticks per second and then we truncate that value between 0 and the `mDuration`. 

```cpp
  float ticksPerSecond = (animation->mTicksPerSecond != 0) ? animation->mTicksPerSecond : 25.0f;
  float timeInTicks = currentTime * ticksPerSecond;
  float animationTime = fmod(timeInTicks, animation->mDuration);
```

where:

```bash
  animationTime ∈ [0, mDuration)
```

The resulting calculated animation time can be used to interpolate between the current keyframe and the next keyframe using the interpolation formula 

```cpp
  factor = (animationTime - previous_keyframe_time) / (next_keyframe_time - previous_keyframe_time);
  position = start + factor * (end - start); // interpolation
```

where: 

```bash
  factor ∈ [0, 1]
```

As an example `mPositionKeys` is a list of all position keyframes of ONE bone across the whole animation. So if a bone moves during the animation, every time the animator set a position key, it gets stored there.

each key contains:

```cpp
  {
    double mTime; // (in ticks)
    aiVector3D mValue; // (position vector)
  }
```

So for one bone you might have:

| Key Index | Time (ticks) | Position |
| --------- | ------------ | -------- |
| 0         | 0            | (0,0,0)  |
| 1         | 20           | (5,0,0)  |
| 2         | 40           | (8,0,0)  |
| 3         | 50           | (10,0,0) |


and for each transformation we will need to interpolate between them so that we have a smooth and correct transition for each keyframe.

```cpp
  if (node has animation channel)
    use interpolated transform from aiNodeAnim
  else
    use aiNode->mTransformation
```

The final formula is this:

  <p align="center">
    <b>FinalBoneMatrix = GlobalInverse ⋅ GlobalAnimatedTransform ⋅ OffsetMatrix</b>
  <p>
And:
  <p align="center">
    <b>v′ = ∑ weights ​( FinalBoneMatrix​ ⋅ v) </b>
  </p>

We already have the `offsetMatrix` and the `weights`. To calculate the `GlobalAnimatedTransform` and `GlobalInverse` we must understand first that:

* **GlobalInverse**

Sometimes the root node has a transform (like scaling from Blender). If we don’t cancel it, everything shifts or scales incorrectly.

So we multiply by:

  <p align="center">
    <b>GlobalInverse = inverse(rootTransform) </b>
  </p>

* **GlobalAnimatedTransform**

To apply the animation transformations we retrive each transformation of each key (`position`, `rotation` and `scale`) which are stored inside **aiNodeAnim** and multiply in order so we can have the resulting **LocalAnimatedTransform** which is **local space**, relative to the parent. 

  <p align="center">
    <b>Local = T ⋅ R ⋅ S</b>
  </p>

Where hierarchy comes in and we compute the **GlobalAnimatedTransform**

  <p align="center">
    <b>GlobalAnimatedTransform = ParentGlobal ⋅ LocalAnimatedTransform </b>
  </p>

| Stage              | What It Is       |
| ------------------ | ---------------- |
| Interpolation      | Local transform  |
| Multiply by parent | Global transform |

Now you may ask what about `ParentGlobal`. We start at the **root bone**, which has no parent, so its `parentGlobal` is the **identity matrix** (meaning no transformation). We compute its global transform as `GlobalRoot = Identity * LocalRoot`, where `LocalRoot` comes from animation interpolation. Then we move to its child and use the root’s computed global transform, so `GlobalChild = GlobalRoot * LocalChild`. This continues down the hierarchy: each bone multiplies its parent’s already-calculated global transform by its own local animated transform. This top-to-bottom multiplication chains all parent movements into the child, ensuring every bone inherits the motion of all bones above it.
