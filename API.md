# Supermarket System REST API Documentation

This documentation specifies the endpoints exposed by the embedded Mongoose C Web MVP Server. 

## 1. Authentication

### `POST /api/login`
Validates credentials and allocates a functional role configuration metric representing the login user bounds.

**Request Payload (JSON):**
```json
{
    "username": "admin",
    "password": "password123"
}
```

**Successful Response (200 OK):**
```json
{
    "status": "ok",
    "role": 1
}
```
*Note: Role `1` equates to `ADMIN`. Role `2` equates to `STAFF`.*

**Failed Response (200 OK or 401 Unauthorized):**
```json
{
    "error": "Invalid credentials"
}
```

## 2. Product Management

### `GET /api/products`
Retrieves a JSON array of all internally queried supermarket goods natively loaded in the C memory database.

**Successful Response (200 OK):**
```json
[
    {
        "id": "P001",
        "name": "Apple",
        "category": "Fruit",
        "supplier": "Farm XYZ",
        "price": 1.50,
        "stock": 100
    }
]
```

### `POST /api/products` (Placeholder definition)
Creates new catalog products. 

**Request Payload (JSON):**
```json
{
    "name": "Banana",
    "category": "Fruit",
    "supplier": "Farm ABC",
    "price": 0.8,
    "stock": 500
}
```

## 3. Member Management

### `GET /api/members`
Retrieves a static JSON array containing the active register lists of supermarket members including their points and spending metrics.

**Successful Response (200 OK):**
```json
[
    {
        "id": "18800000001",
        "name": "John Doe",
        "phone": "18800000001",
        "points": 500,
        "level": 1,
        "total_amount": 105.5
    }
]
```
