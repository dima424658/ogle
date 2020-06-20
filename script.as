float calc(float a, float b)
{
    Sound3D@ sound = Sound3D("test.mp3");
    sound.Play();
    sound.SetVolume(0.3f);
    // Print the value that we received
    Log("Volume: " + sound.GetVolume() * 100 + "\n");
    Log("Received: " + a + ", " + b + "\n");
    
    return a * b;
}
