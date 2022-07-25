using System;

using Rune;

namespace Sandbox
{
    public class Player : Entity
    {
        void OnCreate()
        {
            Console.WriteLine("Player.OnCreate()");
        }

        void OnUpdate(float deltaTime)
        {
            Console.WriteLine($"Player.OnUpdate({deltaTime})");
            
            float speed = 2.0f;
            Vector3 velocity = new Vector3();
            if (Input.IsKeyHeld(Keycode.W))
            {
                velocity.z += 1.0f;
            }
            if (Input.IsKeyHeld(Keycode.S))
            {
                velocity.z += -1.0f;
            }

            velocity = velocity * speed;

            Vector3 translation = Translation;
            translation = translation + velocity;
            Translation = translation;
        }
    }
}