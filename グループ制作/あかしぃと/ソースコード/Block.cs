using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Block : MonoBehaviour
{
    Material material;
    Material normal;
    Material red;
    Material green;
    Renderer block;
    Renderer player;
    Renderer sheet;
    BoxCollision boxCollision;
    Transform target;

    public AudioClip sound;
    private AudioSource audioSource;

    bool swap;
    bool hitBox;

    void Start()
    {
        normal = Resources.Load("Texture/StageBlock/Materials/W2", typeof(Material)) as Material;
        red = Resources.Load("Texture/StageBlock/Materials/R１", typeof(Material)) as Material;
        green = Resources.Load("Texture/StageBlock/Materials/G１", typeof(Material)) as Material;
        block = gameObject.GetComponent<Renderer>();
        player = GameObject.Find("Player").GetComponent<Renderer>();
        sheet = GameObject.Find("Sheet").GetComponent<Renderer>();
        boxCollision = GameObject.Find("BoxCollision").GetComponent<BoxCollision>();
        target = GameObject.Find("Target").GetComponent<Transform>();

        audioSource = gameObject.GetComponent<AudioSource>();

        swap = hitBox = false;
    }

    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Return) || Input.GetKeyDown(KeyCode.JoystickButton1))
        {
            swap = true;
        }

        if (sheet.material.color.a != 0 &&
            block.material.color.b == 0 &&
            (
            sheet.material.color.r == block.material.color.r ||
            sheet.material.color.g == block.material.color.g)
            )
        {
            transform.position = new Vector3(transform.position.x, transform.position.y, 1.0f);
        }
        else
        {
            transform.position = new Vector3(transform.position.x, transform.position.y, .0f);
        }
    }

    void FixedUpdate()
    {
        if (hitBox)
        {
            if (swap)
            {
                Color stay = player.material.color;
                player.material.color = block.material.color;
                block.material.color = stay;

                Material[] materials = block.materials;

                if (block.material.color == Color.white)
                {
                    materials[1] = normal;
                }
                else if (block.material.color == Color.red)
                {
                    materials[1] = red;
                }
                else if (block.material.color == Color.green)
                {
                    materials[1] = green;
                }

                block.materials = materials;

                audioSource.PlayOneShot(sound);
            }
        }

        swap = false;
        hitBox = false;
    }

    void OnTriggerStay(Collider hit)
    {
        if(hit.gameObject.CompareTag("BoxCollision"))
        {
            hitBox = true;
        }
        else if (hit.gameObject.CompareTag("BoxCollisionBottom"))
        {
            if (!boxCollision.HitBlock)
            {
                if (Mathf.Abs(GameObject.Find("Player").transform.position.x - gameObject.transform.position.x) < 0.5f)
                {
                    hitBox = true;
                }
            }
        }

        if(hitBox)
        {
            target.position = new Vector3(gameObject.transform.position.x, gameObject.transform.position.y, -0.05f);
        }
    }

    void OnTriggerExit(Collider hit)
    {
        if (hit.gameObject.CompareTag("BoxCollision") || hit.gameObject.CompareTag("BoxCollisionBottom"))
        {
            target.position = new Vector3(gameObject.transform.position.x, gameObject.transform.position.y, 1f);
        }
    }
}