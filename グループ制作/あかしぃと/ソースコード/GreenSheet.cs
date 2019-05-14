using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GreenSheet : MonoBehaviour
{
    float speed = 20.0f;
    float angle;
    Transform target;
    RedSheet redSheet;
    BoxCollision boxCollision;

    public AudioClip sound;
    private AudioSource audioSource;

    bool useGreen = false;
    bool rotEnd = false;

    void Start()
    {
        target = GameObject.Find("RotSheet").transform;

        redSheet = GameObject.Find("RedSheet").GetComponent<RedSheet>();
        boxCollision = GameObject.Find("BoxCollision").GetComponent<BoxCollision>();

        audioSource = gameObject.GetComponent<AudioSource>();
    }

    void Update()
    {
        if (Input.GetKeyDown(KeyCode.S) || Input.GetKeyDown(KeyCode.JoystickButton4))
        {
            if (!boxCollision.HitBlockCollision)
            {
                if (useGreen)
                {
                    useGreen = false;
                }
                else
                {
                    useGreen = true;
                    redSheet.Use = false;
                }

                audioSource.PlayOneShot(sound);
            }
        }
    }

    void FixedUpdate()
    {
        if (useGreen)
        {
            if (angle > -180.0f)
            {
                transform.RotateAround(target.position, Vector3.forward, -speed);
                angle -= speed;
            }
            else
            {
                rotEnd = true;

                transform.position = new Vector3(transform.position.x, transform.position.y, 1000.0f);
            }
        }
        else
        {
            if (angle < 0.0f)
            {
                transform.RotateAround(target.position, Vector3.forward, speed);
                angle += speed;
            }

            rotEnd = false;

            transform.position = new Vector3(transform.position.x, transform.position.y, -0.14f * 0.7f);
        }
    }

    public bool Use
    {
        get { return useGreen; }
        set { useGreen = value; }
    }

    public bool RotEnd
    {
        get { return rotEnd; }
    }
}