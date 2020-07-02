float calc(float a, float b)
{
  glm::vec3 foo;
  foo.x = 1.0f;
  foo.y = 2.0f;
  foo.z = 3.0f;

  glm::vec3 bar(1.0f, 1.0f, 1.0f);

  glm::vec3 qwe = foo / 2.0f;

  Log(qwe);

   // Sound3D@ sound = Sound3D("test.mp3");
   // sound.Play();
   // sound.SetVolume(0.3f);
    // Print the value that we received
  //  Log("Volume: " + sound.GetVolume() * 100 + "\n");
    Log("Received: " + a + ", " + b + "\n");
    
    return a * b;
}
