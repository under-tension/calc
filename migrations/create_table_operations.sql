CREATE TABLE operations (
    id SERIAL PRIMARY KEY,
    operation_type VARCHAR(50) NOT NULL,
    operand1 NUMERIC NOT NULL,
    operand2 NUMERIC,
    result NUMERIC NOT NULL,
    status NUMERIC NOT NULL
);