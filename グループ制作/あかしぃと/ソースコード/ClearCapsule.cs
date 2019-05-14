using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ClearCapsule : MonoBehaviour
{
    public GameObject clear;
    private Renderer sheet;

    void Start()
    {
        sheet = GameObject.Find("Sheet").GetComponent<Renderer>();
    }

    void Update()
    {
        transform.Rotate(new Vector3(0, 360, 0) * Time.deltaTime, Space.World);
    }

    void FixedUpdate()
    {
        if (sheet.material.color.r != 0 &&
            sheet.material.color.a != 0)
        {
            transform.position = new Vector3(transform.position.x, transform.position.y, 1.0f);
        }
        else
        {
            transform.position = new Vector3(transform.position.x, transform.position.y, 0.0f);
        }
    }

    void OnTriggerEnter(Collider hit)
    {
        if (hit.CompareTag("Player"))
        {
            Destroy(gameObject);

            clear.SetActive(true);
        }
    }
}