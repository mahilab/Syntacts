using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Syntacts;

public class BouncyBall : MonoBehaviour
{
    public SyntactsDemo demo;   
    private Rigidbody rb;

    public int collisionChannel = 0;
    public int velocityChannel = 1;

    public float collisionFreq = 500;
    public float velocityFreq = 200;


    // Start is called before the first frame update
    void Start()
    {
        demo.session.Play(velocityChannel, new Sine(velocityFreq) * new Sine(5));
        rb = GetComponent<Rigidbody>();
    }

    // Update is called once per frame
    void Update()
    {
        demo.session.SetPitch(velocityChannel, 1 + rb.velocity.magnitude* 0.1);
    }

    void OnCollisionEnter(Collision col) {
        Signal collision = new Square(collisionFreq) * new ASR(0.05, 0.05, 0.05);
        demo.session.Play(collisionChannel, collision);
    }
}
