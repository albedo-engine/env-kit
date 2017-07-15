#include <gtest/gtest.h>

TEST(Cubemap, GetPixel)
{

  /*std::vector<std::shared_ptr<tiny3Dloader::scene::Scene>> scenes;
  tiny3Dloader::Importer importer;
  bool status = importer.load("models/gltf/Box.gltf", "models/gltf/", scenes);
  EXPECT_TRUE(status);
  // Checks the number of scenes
  EXPECT_EQ(1, scenes.size());
  // Checks the number of nodes in the scene
  const auto& scenePtr = scenes[0];
  EXPECT_EQ(1, scenePtr->nodes.size());
  // Checks the root node
  const auto& rootPtr = scenePtr->nodes[0];
  EXPECT_EQ(1, rootPtr->children.size());
  for (const auto& nodePtr : rootPtr->children) {
    EXPECT_EQ(0, nodePtr->children.size());
  }
  const auto& meshNode = rootPtr->children[0];
  EXPECT_EQ(1, meshNode->meshes.size());
  EXPECT_EQ(1, meshNode->meshes[0]->primitives.size());*/
}

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
