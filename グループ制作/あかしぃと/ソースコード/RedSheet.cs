using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RedSheet : MonoBehaviour
{
    float speed = 20.0f;
    float angle;
    Transform target;
    GreenSheet greenSheet;
    BoxCollision boxCollision;

    public AudioClip sound;
    private AudioSource audioSource;

    bool useRed = false;
    bool rotEnd = false;

    void Start()
    {
        target = GameObject.Find("RotSheet").transform;

        greenSheet = GameObject.Find("GreenSheet").GetComponent<GreenSheet>();
        boxCollision = GameObject.Find("BoxCollision").GetComponent<BoxCollision>();

        audioSource = gameObject.GetComponent<AudioSource>();
    }

    void Update()
    {
        if (Input.GetKeyDown(KeyCode.W) || Input.GetKeyDown(KeyCode.JoystickButton5))
        {
            if (!boxCollision.HitBlockCollision)
            {
                if (useRed)
                {
                    useRed = false;
                }
                else
                {
                    useRed = true;
                    greenSheet.Use = false;
                }

                audioSource.PlayOneShot(sound);
            }
        }
    }

    void FixedUpdate()
    {
        if (useRed)
        {
            if (angle < 180.0f)
            {
                transform.RotateAround(target.position, Vector3.forward, speed);
                angle += speed;
            }
            else
            {
                rotEnd = true;

                transform.position = new Vector3(transform.position.x, transform.position.y, 1000.0f);
            }
        }
        else
        {
            if (angle > 0.0f)
            {
                transform.RotateAround(target.position, Vector3.forward, -speed);
                angle -= speed;
            }

            rotEnd = false;

            transform.position = new Vector3(transform.position.x, transform.position.y, -0.12f * 0.7f);
        }
    }

    public bool Use
    {
        get { return useRed; }
        set { useRed = value; }
    }

    public bool RotEnd
    {
        get { return rotEnd; }
    }
}